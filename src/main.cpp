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
    int numThreads;
    int batchSize;
    int maxSolutions;
    shellPrepSolver(numThreads, batchSize, maxSolutions);
    Solutions solutions(maxSolutions);
    Solutions thread_solutions(maxSolutions);
    slvr::Solver solver(pieceList, solutions, thread_solutions, numThreads, batchSize);
    runSolver(solver, pieceList, numThreads > 0);

    //Stage 4: View results
    stage4(solver, pieceList) {
        std::cout << "How would you like to view the results?" << std::endl;
        shellDisplayResults(solver, pieceList);
    }

    
    //acceptUserInputAndDisplayCorrespondingInformation();   
    
}