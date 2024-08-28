#include "../include/solver.hpp"

#include "../include/solutions.hpp"

#define USE_BATCHING true
#define USE_BOUNDINGBOX true

using Time = std::chrono::steady_clock;

namespace slvr {

    void Solver::prepBoard() {
        for (int i = 0; i < 10 * 10; ++i) {
            task.board[i] = ' ';
        }
    }

    void Solver::prepLocations() {
        for (int i = 0; i < 10; i++) 
            for (int j = 0; j < 10; j++)
                locations.push_back(std::make_pair(i, j));
    }

    inline bool inBounds(const Location& location, const pcs::BB& bb) {
        int x = location.first, y = location.second;
        return bb.left + x >= 0 && bb.right + x < 10 && bb.up + y >= 0 && bb.down + y < 10;
    }
    std::vector<int> Solver::checkPlacement(const pcs::Piece* piece, const Location& location, const Board* board) {
        //return true for the orientations that would fit at current Location

        std::vector<int> ret;
        bool placeable;

        for (int i = 0; i < piece->orientations.size(); i++) {
            const pcs::Orientation& orientation = piece->orientations[i];
            if (!inBounds(location, orientation.boundingBox)) {
                continue;
            }
            placeable = true;
            for (const Location& coordinate : orientation.coordinates) {
                int row = coordinate.first + location.first;
                int col = coordinate.second + location.second;
                /*if (row < 0 || row >= 10 || col < 0 || col >= 10) { //maybe similar optimization here as in placePiece()?
                    placeable = false;
                    break;
                }*/
                if ((*board)[row * 10 + col] != ' ') { //which would likely affect this as well
                    placeable = false;
                    break;
                }//note that these previous two conditions could be merged bc they do the same thing - just keeping them separate for now
                //because of the possibility of future optimization, in which case I want it to be easily visible
            }
            if (placeable) ret.push_back(i);
        }
        return ret;
    }

    //Modifies the board!
    //Please make sure you verify checkPlacement for this particular orientation
    void Solver::placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Board* board) {
        int idx_offset = 10 * location.first + location.second;
        for (const Location& coordinate : piece->orientations[orientation_idx].coordinates) {
            //int row = coordinate.first + location.first;
            //int col = coordinate.second + location.second;
            //int idx = row * 10 + col; // = (coordinate.first + location.first) * 10 + coordinate.second + location.second
            int idx = 10 * coordinate.first + coordinate.second + idx_offset;
            (*board)[idx] = piece->symbol;
        }
    }

    void Solver::placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task) {
        int idx_offset = 10 * location.first + location.second;
        for (const Location& coordinate : piece->orientations[orientation_idx].coordinates) {
            int idx = 10 * coordinate.first + coordinate.second + idx_offset;
            task->board[idx] = piece->symbol;
        }
        task->current_state.push_back(std::make_pair(location, orientation_idx));
    }

    void Solver::removePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task) {
        //assert(task->current_state.back().first == location && task->current_state.back().second == orientation_idx)
        //maybe include the above line in a debugging mode

        int idx_offset = 10 * location.first + location.second;

        for (const Location& coordinate : piece->orientations[orientation_idx].coordinates) {
            //int row = coordinate.first + location.first;
            //int col = coordinate.second + location.second;
            //int idx = row * 10 + col; // = (coordinate.first + location.first) * 10 + coordinate.second + location.second
            int idx = 10 * coordinate.first + coordinate.second + idx_offset;
            task->board[idx] = ' ';
        }
        task->current_state.pop_back();
    }

    Solver::Solver(const std::vector<std::string>& pieceList, Solutions& solutions, Solutions& thread_solutions, int numThreads, 
    size_t batchSize) 
    : pieceList(pieceList), solutions(solutions), thread_solutions(thread_solutions), numThreads(numThreads), batchSize(batchSize),
    useBatching(batchSize > 0) {
        prepBoard();
        prepLocations();
    }

    void Solver::solve(const int pc_idx) { //default value already given in header file, are we allowed it here, and will it give an error?
        const pcs::Piece* piece = pcs::piece_names.at(pieceList[pc_idx]);
        for (const Location& location : locations) {
            
            const auto start1 = Time::now();
            std::vector<int> result = checkPlacement(piece, location, &(task.board));
            const auto end1 = Time::now();
            benchmark1 += end1-start1;
            numCalls1++;


            for (int i : result) {
                placePiece(piece, i, location, &task);
                if (pc_idx < pieceList.size() - 1) {
                    solve(pc_idx + 1);
                } else {
                    solutions.addSolution(task.current_state);//in multithreaded solving, we would use the Task passed in by reference
                        //but for now we use the Solver's Task
                }
                if (solutions.maxSolutionsReached()) return;
                removePiece(piece, i, location, &task);
            }
        }
    }























    









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

    void Solver::execute(const Task& task, std::vector<Solution>& newSolutions) { //use this for the last piece
        const int pc_idx = task.current_state.size();
        const pcs::Piece* piece = pcs::piece_names.at(pieceList[pc_idx]);
        for (const Location& location : locations) {
            std::vector<int> result = checkPlacement(piece, location, &(task.board));
            for (int i : result) {
                Solution solution = task.current_state;
                solution.push_back(std::make_pair(location, i));
                newSolutions.push_back(std::move(solution)); //now temp's current_state is empty lol
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

            ///consumer: get task
            pthread_mutex_lock(&(self->queueLock));
            while (self->taskQueue.size() == 0 && self->activeThreads > 0) {
                pthread_cond_wait(&(self->queueCond), &(self->queueLock));
            }
            if (self->taskQueue.size() == 0) {
                pthread_cond_broadcast(&(self->queueCond)); //wake the sleeping threads
                pthread_mutex_unlock(&(self->queueLock)); //can't forget about this!
                break; //exit while loop
            }
            if (self->useBatching) {
            //#if USE_BATCHING
                numTasksToMove = std::min(self->taskQueue.size(), self->batchSize);
                for (int i = 0; i < numTasksToMove; ++i) {
                    tasksToProcess.push_back(std::move(self->taskQueue.front()));
                    self->taskQueue.pop_front();
                }
            } else {
            //#else
                task = std::move(self->taskQueue.front());
                self->taskQueue.pop_front();
            }
            //#endif
            self->activeThreads++; //this thread took a Task or batch of Tasks, so it's active!
            pthread_mutex_unlock(&(self->queueLock));
            
            ///executor: execute task
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

            ///producer: add tasks and solution
            pthread_mutex_lock(&(self->queueLock));
            self->taskQueue.splice(self->taskQueue.end(), newTasks);
            self->thread_solutions.addSolutions(newSolutions);
            self->activeThreads--; //thread is no longer going to add Tasks (until it picks up new ones) so it is no longer active!
            pthread_cond_broadcast(&(self->queueCond)); //wake up the other threads to take what they can      
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

        /*
        usleep(10000000);
        pthread_mutex_lock(&queueLock);
        std::cout << "Solutions found so far: " << std::to_string(thread_solutions.getNumSolutions()) << std::endl;
        pthread_mutex_unlock(&queueLock);
        */

        for (int i = 0; i < numThreads; ++i) 
            if (pthread_join(threadPool[i], NULL) != 0) 
                std::cerr << "Failed to join thread" << std::endl;   

        pthread_mutex_destroy(&queueLock);
        pthread_cond_destroy(&queueCond);
    }
}