#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <list>
#include <pthread.h>
#include <utility>
#include <vector>

#include "piece.hpp"
#include "task.hpp"

using Location = std::pair<int, int>;
using Solution = std::vector<std::pair<Location, int>>;
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
        Solver(const std::vector<std::string>& pieceList, Solutions& solutions, Solutions& thread_solutions, int numThreads, size_t batchSize);

        //getters
        Solutions& getSolutions() const {return solutions;}
        Solutions& thread_getSolutions() const {return thread_solutions;}
        const std::vector<std::string>& getPieceList() const {return pieceList;}

        //tools during solving
        static void placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task);
        static void placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Board* board);
        static void removePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task);

        //single-threaded
        void solve(const size_t pc_idx = 0);

        //multi-threaded methods
        void thread_solve();
        static void* startup(void* args);
        void execute(const Task& task, std::list<Task>& newTasks);
        void execute(const Task& task, std::vector<Solution>& newSolutions);

        //multi-threaded parameters
        int numThreads;
        size_t batchSize;
        bool useBatching;

        //fields used by threads
        std::list<Task> taskQueue;
        pthread_mutex_t queueLock = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t queueCond = PTHREAD_COND_INITIALIZER;
        int activeThreads = 0;
        
        
    };
}

#endif
