#include <string>
#include <vector>
#include "../include/solutions.hpp"
#include "../include/stages.hpp"

int main() {
    //Stage 0: Initial condition checking
    stage0();

    //Stage 1: Create/edit piece list
    std::vector<std::string> pieceList = shellCreatePieceList();
    sortPieceList(pieceList);

    //Stage 2: Run solver
    int numThreads, batchSize, maxSolutions;
    shellPrepSolver(numThreads, batchSize, maxSolutions);
    Solutions solutions(maxSolutions), thread_solutions(maxSolutions);
    slvr::Solver solver(pieceList, solutions, thread_solutions, numThreads, batchSize);
    runSolver(solver);

    //Stage 3: View results
    shellDisplayResults(solver);    
}