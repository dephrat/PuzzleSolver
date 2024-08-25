#include <string>
#include <vector>
#include "../include/solutions.hpp"
#include "../include/stages.hpp"

int main() {
    //Stage 0: Initial condition checking
    //if (!piecesValidation()) exit(0);
    //generatePieceOrientations();
    stage0();

    //Stage 1: Create/edit piece list
    std::vector<std::string> pieceList = shellCreatePieceList();


    //Stage 2: Run solver
    //promptUserForInput(); //optimization input
    int numThreads;
    int batchSize;
    int maxSolutions;
    shellPrepSolver(numThreads, batchSize, maxSolutions);

    //int maxSolutions = shellPrepSolver();
    //performNecessaryInitializations();
        //includes Solutions for our storage
    Solutions solutions(maxSolutions);
    Solutions thread_solutions(maxSolutions);
    
    //setNecessaryGlobalOptimizationVariables();
    slvr::Solver solver(pieceList, solutions, thread_solutions, 64, 20);
    runSolver(solver, pieceList);

    /*
    //Stage 3: Wait for solver to finish
    waitForSolverToFinish();
    std::cout << "Solving complete!" << std::endl;
    */

    //Stage 4: View results
    std::cout << "How would you like to view the results?" << std::endl;
    shellDisplayResults(solver, pieceList);
    //acceptUserInputAndDisplayCorrespondingInformation();

    std::cout << solutions.getNumSolutions() << std::endl;

    
    
}