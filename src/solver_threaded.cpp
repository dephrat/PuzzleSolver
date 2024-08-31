#include "../include/solver.hpp"

#include "../include/solutions.hpp"

//Multi-threaded approach:

namespace slvr {

    void Solver::execute(const Task& task, std::list<Task>& newTasks) {
        //Identify the index of the current piece to be added
        const size_t pc_idx = task.current_state.size();

        //Get that piece from the piece list
        const pcs::Piece* piece = pcs::piece_names.at(pieceList[pc_idx]);

        //Check everywhere you could place it
        for (const Location& location : locations) {
            std::vector<int> result = checkPlacement(piece, location, &(task.board));
            for (int i : result) {
                //Copy the Task to a new Task, update it, and add it to our return list
                Task temp = task;
                placePiece(piece, i, location, &temp); 
                newTasks.push_back(std::move(temp));
            }
        }
        return;
    }

    void Solver::execute(const Task& task, std::vector<Solution>& newSolutions) {
        //Identify the index of the current piece to be added
        const size_t pc_idx = task.current_state.size();

        //Get that piece from the piece list
        const pcs::Piece* piece = pcs::piece_names.at(pieceList[pc_idx]);

        //Check everywhere you could place it
        for (const Location& location : locations) {
            std::vector<int> result = checkPlacement(piece, location, &(task.board));
            for (int i : result) {
                //We're trying to place the final piece, so we only need to update the Solution from the current Task
                Solution solution = task.current_state;
                solution.push_back(std::make_pair(location, i));
                newSolutions.push_back(std::move(solution));
            }
        }
        return;
    }

    void* Solver::startup(void* args) {
        Solver* self = static_cast<Solver*>(args);
        while (true) {
            //Setup variables
            int numTasksToMove = 0;
            std::vector<Task> tasksToProcess;
            Task task;

            //Consumer: get task
            pthread_mutex_lock(&(self->queueLock));

            /* Logic for waiting/exiting/continuing:
            s = need more solutions
            t = tasks available
            a = there are active threads
            sta action
            000 finish (no more work needs to be done)
            001 finish ``
            010 finish ``
            011 finish ``
            100 finish (no more work can be done, there are no active threads to generate more tasks)
            101 go to sleep (wait for tasks from the active threads)
            110 continue (go get a task)
            111 continue ``
            */
            
            //We need tasks, there are none, but there are active threads.
            //Therefore, wait for the active threads to deposit their tasks and/or solutions.
            while (self->thread_solutions.maxSolutionsReached() == false && self->taskQueue.size() == 0 && self->activeThreads > 0) {
               pthread_cond_wait(&(self->queueCond), &(self->queueLock));
            }

            //Quit early if we've found enough solutions or if we're out of tasks and not going to get more.
            //Note: I could make these if-else statements more concise, but I wanted to preserve the clarity of logic and make it easy to understand.
            if (self->thread_solutions.maxSolutionsReached() == true) {//our work here is done, it's time to go home.
                pthread_cond_broadcast(&(self->queueCond));
                pthread_mutex_unlock(&(self->queueLock));
                break;
            } else if (self->taskQueue.size() > 0) {//we need solutions, and we have tasks! Go get em soldier!
                //do nothing
            } else if (self->activeThreads == 0) { //no active threads means no more tasks. We need solutions, but we're not getting em.
                pthread_cond_broadcast(&(self->queueCond));
                pthread_mutex_unlock(&(self->queueLock));
                break;
            }

            //Take tasks from the queue
            if (self->useBatching) {
                numTasksToMove = std::min(self->taskQueue.size(), self->batchSize);
                for (int i = 0; i < numTasksToMove; ++i) {
                    tasksToProcess.push_back(std::move(self->taskQueue.front()));
                    self->taskQueue.pop_front();
                }
            } else {
                task = std::move(self->taskQueue.front());
                self->taskQueue.pop_front();
            }
            self->activeThreads++; //this thread took a Task or batch of Tasks, so it's active!
            pthread_mutex_unlock(&(self->queueLock));

            //Executor: execute task(s)
            std::vector<Solution> newSolutions;
            std::list<Task> newTasks;
            if (self->useBatching) {
                for (int i = 0; i < numTasksToMove; ++i) {
                    
                    if (tasksToProcess[i].current_state.size() == self->pieceList.size() - 1)
                        //If we're placing the final piece, we're finding solutions
                        self->execute(tasksToProcess[i], newSolutions);
                    else
                        //Otherwise, we're finding intermediate Tasks
                        self->execute(tasksToProcess[i], newTasks);
                }
            } else {
                if (task.current_state.size() == self->pieceList.size() - 1)
                    //If we're placing the final piece, we're finding solutions
                    self->execute(task, newSolutions);
                else
                    //Otherwise, we're finding intermediate Tasks
                    self->execute(task, newTasks);
            }

            //Producer: add tasks and solution
            pthread_mutex_lock(&(self->queueLock));

            //Add newly discovered tasks and solutions
            self->taskQueue.splice(self->taskQueue.begin(), newTasks); //add to front to catch early solutions
            self->thread_solutions.addSolutions(newSolutions);
            self->activeThreads--; //thread is no longer going to add Tasks (until it picks up new ones) so it is no longer active!
            
            //Prepare to go back to being a consumer
            pthread_cond_broadcast(&(self->queueCond)); //wake up the other threads to take what they can  
            pthread_mutex_unlock(&(self->queueLock));
        }
        return NULL;
    }

    void Solver::thread_solve() {
        //Prepare initial task
        taskQueue.emplace_back(task);

        //Prepare thread synchronization variables
        pthread_mutex_init(&queueLock, NULL);
        pthread_cond_init(&queueCond, NULL);

        //Create thread pool
        pthread_t threadPool[numThreads];

        //Start thread execution
        for (int i = 0; i < numThreads; ++i) 
            if (pthread_create(&threadPool[i], NULL, startup, this) != 0) 
                std::cerr << "Failed to create thread" << std::endl; 

        //Collect thread execution
        for (int i = 0; i < numThreads; ++i) 
            if (pthread_join(threadPool[i], NULL) != 0) 
                std::cerr << "Failed to join thread" << std::endl;   

        //Clean up thread synchronization variables
        pthread_mutex_destroy(&queueLock);
        pthread_cond_destroy(&queueCond);
    }
}