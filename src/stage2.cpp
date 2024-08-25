#include "../include/stages.hpp"

int shellPrepSolver() {
    int maxSolutions;
    while (true) {
        std::cout << "How many solutions? (-1 for unlimited): ";
        if (!(std::cin >> maxSolutions)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << msgs::invalidIntegerInputNumSolutionsErrorMessage << std::endl;
            continue;
        } else if (maxSolutions < -1) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << msgs::invalidIntegerInputNumSolutionsErrorMessage << std::endl;
            continue;
        } else return maxSolutions;
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
    std::cout << "\nThreaded solving finished. Time taken (s): " << fp_ms.count()/1000 << std::endl;

    const auto start1 = Time::now();
    //single-threaded
    solver.solve();
    const auto end1 = Time::now();
    fp_ms = end1 - start1;
    std::cout << "\nSolving finished. Time taken (s): " << fp_ms.count()/1000 << std::endl;

    
    

    

}