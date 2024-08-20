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

void runSolver(slvr::Solver& solver, const std::vector<std::string>& pieceList, Solutions& solutions) {
    using Time = std::chrono::steady_clock;
    std::chrono::duration<double, std::milli> fp_ms;
    const auto start = Time::now();

    //start solving
    if (pieceList.size())
        solver.solve();
    //end solving

    const auto end = Time::now();
    fp_ms = end - start;
    std::cout << "Solving finished. Time taken (s): " << fp_ms.count()/1000 << std::endl;
}