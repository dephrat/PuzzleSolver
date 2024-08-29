#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../include/solutions.hpp"
#include "../include/stage0.hpp"
#include "../include/stage1.hpp"
#include "../include/stage2.hpp"
#include "../include/stage3.hpp"

int main() {
    //Stage 0: Initial condition checking
    stage0();

    //Stage 1: Generate piece list
    std::vector<std::string> pieceList = shellCreatePieceList();
    sortPieceList(pieceList);

    //Stage 2: Run solver
    int numThreads, batchSize, maxSolutions;
    shellPrepSolver(numThreads, batchSize, maxSolutions);
    Solutions solutions(maxSolutions), thread_solutions(maxSolutions);
    slvr::Solver solver(pieceList, solutions, thread_solutions, numThreads, batchSize);

    //Benchmark solve time:
    using Time = std::chrono::steady_clock;
    std::chrono::duration<double, std::milli> fp_ms;
    const auto start = Time::now();
    runSolver(solver);
    const auto end = Time::now();
    fp_ms = end - start;
    auto solveTime = fp_ms.count() / 1000;
    std::cout << "Solving finished. Time taken (s): " << solveTime << std::endl;

    //Stage 3: View results
    shellDisplayResults(solver);    
}