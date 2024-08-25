#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <array>
#include <utility>
#include <vector>


//THREADED STUFF
#include <algorithm>
#include <iostream>
#include <list>
#include <pthread.h>
#include <unistd.h> 
//END OF THREADED STUFF


#include "piece.hpp"
#include "task.hpp"

using Location = std::pair<int, int>;
using Solution = std::vector<std::pair<Location, int>>; //pair<Location, orientation index>, can reconstruct a game state using this and a Board
using Board = std::array<char, 10 * 10>;

class Solutions;

namespace slvr {

    class Solver {
    private:

        const std::vector<std::string> pieceList;
        Solutions& solutions;
        Solutions& thread_solutions;
        Task task;
        std::vector<Location> locations;

        void prepBoard();
        void prepLocations();
        std::vector<int> checkPlacement(const pcs::Piece* piece, const Location& location, const Board* board);

    public:
        Solutions& getSolutions() const {return solutions;}
        Solutions& thread_getSolutions() const {return thread_solutions;}

        static void placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task);
        static void placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Board* board);
        static void removePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task);

        //Solver(const std::vector<std::string>& pieceList, int maxSolutions = 1); Changed Solver's solutions to a reference
        Solver(const std::vector<std::string>& pieceList, Solutions& solutions, Solutions& thread_solutions, int numThreads = 1, size_t batchSize = 1);
        Solver(const std::vector<std::string>& pieceList, Solutions& solutions, int numThreads = 1, size_t batchSize = 1);
        void solve(const int pc_idx = 0);

        static void* startup(void* args);

        void execute(const Task& task, std::list<Task>& newTasks);
        void execute(const Task& task, std::vector<Solution>& newSolutions);


        std::list<Task> taskQueue;
        pthread_mutex_t queueLock = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t queueCond = PTHREAD_COND_INITIALIZER;
        int activeThreads = 0;
        int numThreads;
        size_t batchSize;
        void thread_solve();


        std::chrono::duration<double, std::milli> benchmark1;
        std::chrono::duration<double, std::milli> benchmark2;
        std::chrono::duration<double, std::milli> benchmark3;
        long int numCalls1 = 0;
    };
}

#endif
