#include <string>
#include <vector>
#include "../include/solutions.hpp"
#include "../include/stages.hpp"

int main() {
    //Stage 0: Initial condition checking
    if (!piecesValidation()) exit(0);
    generatePieceOrientations();

    //Stage 1: Create/edit piece list
    std::vector<std::string> pieceList = shellCreatePieceList();


    //Stage 2: Run solver
    //promptUserForInput(); //optimization input
    int maxSolutions = shellPrepSolver();
    //performNecessaryInitializations();
        //includes Solutions for our storage
    Solutions solutions(maxSolutions);
    
    //setNecessaryGlobalOptimizationVariables();
    slvr::Solver solver(pieceList, solutions);
    runSolver(solver, pieceList, solutions);

    /*
    //Stage 3: Wait for solver to finish
    waitForSolverToFinish();
    std::cout << "Solving complete!" << std::endl;
    */

    //Stage 4: View results
    std::cout << "How would you like to view the results?" << std::endl;
    shellDisplayResults(solutions, pieceList);
    //acceptUserInputAndDisplayCorrespondingInformation();

    std::cout << solutions.getNumSolutions() << std::endl;

    
    
}