#include "../include/solver.hpp"

#include "../include/solutions.hpp"

//Multi-threaded approach:

namespace slvr {

    void Solver::execute(const Task& task, std::list<Task>& newTasks) {
        const int pc_idx = task.current_state.size();
        const pcs::Piece* piece = pcs::piece_names.at(pieceList[pc_idx]);
        for (const Location& location : locations) {
            std::vector<int> result = checkPlacement(piece, location, &(task.board));
            for (int i : result) {
                Task temp = task;
                placePiece(piece, i, location, &temp); 
                newTasks.push_back(std::move(temp));
            }
        }
        return;
    }

    void Solver::execute(const Task& task, std::vector<Solution>& newSolutions) {
        const int pc_idx = task.current_state.size();
        const pcs::Piece* piece = pcs::piece_names.at(pieceList[pc_idx]);
        for (const Location& location : locations) {
            std::vector<int> result = checkPlacement(piece, location, &(task.board));
            for (int i : result) {
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
            int numTasksToMove = 0;
            std::vector<Task> tasksToProcess;
            Task task;
            //consumer: get task
            pthread_mutex_lock(&(self->queueLock));
            /*
            need more solutions, there are tasks, there are active threads
            000 exit
            001 exit
            010 exit 
            011 exit 
            100 exit
            101 go back to sleep
            110 go get em
            111 go get em

            while (need more tasks && no tasks && there are active threads) {
                wait();
            }
            if (dont need more tasks) {
                broadcast();
                unlock;
                break;
            }
            else if (need more tasks and there are some) {
                go get them //do nothing
            }
            else if (need more tasks but there are none, and theres no active threads) {
                broadcast();
                unlock;
                break;
            }
            */
            while (self->thread_solutions.maxSolutionsReached() == false && self->taskQueue.size() == 0 && self->activeThreads > 0) {
               pthread_cond_wait(&(self->queueCond), &(self->queueLock));
            }
            if (self->thread_solutions.maxSolutionsReached() == true) {//our work here is done, it's time to go home.
                pthread_cond_broadcast(&(self->queueCond));
                pthread_mutex_unlock(&(self->queueLock));
                break;
            }
            else if (self->taskQueue.size() > 0) {//we need solutions, and we have tasks! Go get em soldier!
                //do nothing
            }
            else if (self->activeThreads == 0) { //no active threads means no more tasks. We need solutions, but we're not getting em.
                pthread_cond_broadcast(&(self->queueCond));
                pthread_mutex_unlock(&(self->queueLock));
                break;
            }


            /*while (self->taskQueue.size() == 0 && self->activeThreads > 0) {
                pthread_cond_wait(&(self->queueCond), &(self->queueLock));
            }*/
/*
            if (self->taskQueue.size() == 0) { //done executing
                pthread_cond_broadcast(&(self->queueCond)); //wake the sleeping threads
                pthread_mutex_unlock(&(self->queueLock));
                break;
            }
*/
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
            //executor: execute task
            std::vector<Solution> newSolutions;
            std::list<Task> newTasks;
            if (self->useBatching) {
                for (int i = 0; i < numTasksToMove; ++i) {
                    if (tasksToProcess[i].current_state.size() == self->pieceList.size() - 1) { //last piece
                        self->execute(tasksToProcess[i], newSolutions);
                    } else {
                        self->execute(tasksToProcess[i], newTasks);
                    }
                }
            } else {
                if (task.current_state.size() == self->pieceList.size() - 1) { //last piece
                    self->execute(task, newSolutions);
                } else {
                    self->execute(task, newTasks);
                }
            }
            //producer: add tasks and solution
            pthread_mutex_lock(&(self->queueLock));
            //self->taskQueue.splice(self->taskQueue.end(), newTasks);
            self->taskQueue.splice(self->taskQueue.begin(), newTasks);
            self->thread_solutions.addSolutions(newSolutions);
            self->activeThreads--; //thread is no longer going to add Tasks (until it picks up new ones) so it is no longer active!
            pthread_cond_broadcast(&(self->queueCond)); //wake up the other threads to take what they can  

            /*bool timeToGo = self->thread_solutions.maxSolutionsReached();
            pthread_mutex_unlock(&(self->queueLock));
            if (timeToGo) {
                break;
            }*/
/*
            if we have enough solutions, we want each thread to leave.
            there are these kinds of threads to deal with before we exit (they dont have the lock):
                waiting for consumer lock
                sleeping
                waiting for post-wake-up lock 
                waiting on producer lock 

            idea: in consumer lock, dont bother getting a task, just wake up and leave. 
                    //in producer lock, after placing tasks in, check numSolutions and then leave.
                
*/

            pthread_mutex_unlock(&(self->queueLock));
        }
        return NULL;
    }

    void Solver::thread_solve() {
        taskQueue.emplace_back(task);
        pthread_mutex_init(&queueLock, NULL);
        pthread_cond_init(&queueCond, NULL);
        pthread_t threadPool[numThreads];
        for (int i = 0; i < numThreads; ++i) 
            if (pthread_create(&threadPool[i], NULL, startup, this) != 0) 
                std::cerr << "Failed to create thread" << std::endl; 
        for (int i = 0; i < numThreads; ++i) 
            if (pthread_join(threadPool[i], NULL) != 0) 
                std::cerr << "Failed to join thread" << std::endl;   
        pthread_mutex_destroy(&queueLock);
        pthread_cond_destroy(&queueCond);
    }
}