#include "../include/stages.hpp"

void shellPrepSolver(int& numThreads, int& batchSize, int& maxSolutions) {
    int nT;
    while (true) {
        std::cout << "How many threads? (0 for single-threaded): ";
        if (!(std::cin >> nT)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << msgs::invalidIntegerInputNumThreadsErrorMessage << std::endl;
            continue;
        } else if (nT < 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << msgs::invalidIntegerInputNumThreadsErrorMessage << std::endl;
            continue;
        } else {
            numThreads = nT;
            break;
        }
    }
    if (numThreads) {
        int bS;
        while (true) {
            std::cout << "Thread batch size?: ";
            if (!(std::cin >> bS)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << msgs::invalidIntegerInputBatchSizeErrorMessage << std::endl;
                continue;
            } else if (bS < 1) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << msgs::invalidIntegerInputBatchSizeErrorMessage << std::endl;
                continue;
            } else {
                batchSize = bS;
                break;
            }
        }
    } else batchSize = 0;
    int mS;
    while (true) {
        std::cout << "How many solutions? (-1 for unlimited): ";
        if (!(std::cin >> mS)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << msgs::invalidIntegerInputNumSolutionsErrorMessage << std::endl;
            continue;
        } else if (mS < -1) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << msgs::invalidIntegerInputNumSolutionsErrorMessage << std::endl;
            continue;
        } else {
            maxSolutions = mS;
            break;
        }
    }
}

void runSolver(slvr::Solver& solver, const std::vector<std::string>& pieceList) {

    if (pieceList.size() == 0) {
        return;
    }

    using Time = std::chrono::steady_clock;
    std::chrono::duration<double, std::milli> fp_ms;

    const auto start2 = Time::now();
    //multi-threaded
    solver.thread_solve();
    const auto end2 = Time::now();
    fp_ms = end2 - start2;
    std::cout << "Threaded solving finished. Time taken (s): " << fp_ms.count()/1000 << std::endl;

    const auto start1 = Time::now();
    //single-threaded
    solver.solve();
    const auto end1 = Time::now();
    fp_ms = end1 - start1;
    std::cout << "Solving finished. Time taken (s): " << fp_ms.count()/1000 << std::endl;

    
    std::cout << "Average benchmark 1: " << (solver.benchmark1.count()/1000) / solver.numCalls1 << std::endl;

    

}