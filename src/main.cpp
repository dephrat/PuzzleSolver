#include <string>
#include <vector>
#include "../include/solutions.hpp"
#include "../include/stages.hpp"

#include <fstream>

int main() {
    //Stage 0: Initial condition checking
    stage0();

    //open file 
    std::ifstream inputFile("thread_input.txt");
    if (!inputFile) {
        std::cerr << "Couldn't open input file" << std::endl;
        return 1;
    }
    

    //direct cin to read from the file
    std::cin.rdbuf(inputFile.rdbuf());

    double averageTime = 0;
    std::vector<double> solveTimes;


    int numIterations = 100;
    for (int i = 0; i < numIterations; ++i) {
        std::cout << "Starting benchmark iteration " << std::to_string(i) << std::endl;
        //Stage 1: Create/edit piece list
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
        averageTime += solveTime;
        solveTimes.push_back(solveTime);


        //Stage 3: View results
        shellDisplayResults(solver);    



        inputFile.clear();
        //reset file pointer
        inputFile.seekg(0, std::ios::beg);
    }

    averageTime /= numIterations;
    double variance = 0;
    for (auto x : solveTimes) {
        variance += (x-averageTime) * (x-averageTime);
    }
    variance /= numIterations;
    std::cout << "Average time: " << std::to_string(averageTime) << std::endl;
    std::cout << "Variance: " << std::to_string(variance) << std::endl;
}

//single threaded: 4.3, var 3.something