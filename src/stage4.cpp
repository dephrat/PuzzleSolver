#include "../include/stages.hpp"

#include "../include/messages.hpp"
#include "../include/solutions.hpp"
#include <iostream>

/*
options:
View 1st solution - solution 0 || s 0
View solution by index - solution <idx> || s <idx>
View number of solutions - numsolutions || ns
View all solutions - all
- show number of solutions, ask to confirm, maybe if the number is large (>100) or will take a while
View piecelist
option to view as board vs as piece placements?
*/
void shellDisplayResults(const slvr::Solver& solver) {
    std::cout << msgs::stage4IntroMessage << std::endl;

    const Solutions& solutions = solver.getSolutions();
    const Solutions& thread_solutions = solver.thread_getSolutions();
    const std::vector<std::string>& pieceList = solver.getPieceList();

    int numSolutions = solutions.getNumSolutions();
    const std::vector<Solution>& solutionsVector = solutions.getSolutions();
    std::vector<Board> boardsVector(numSolutions);
    std::vector<int> boardsPopulatedVector(numSolutions, 0);

    int thread_numSolutions = thread_solutions.getNumSolutions();
    const std::vector<Solution>& thread_solutionsVector = thread_solutions.getSolutions();
    std::vector<Board> thread_boardsVector(thread_numSolutions);
    std::vector<int> thread_boardsPopulatedVector(thread_numSolutions, 0);



    if (numSolutions == 0 && thread_numSolutions == 0) {
        std::cout << "Looks like there were no solutions found. Exiting program by default." << std::endl;
        return;
    }

    std::string input;
    while (true) {
        std::cout << "Your command: ";
        if (std::cin >> input) {
            if (input == "help" || input == "h") {
                std::cout << msgs::stage4CommandsMessage << std::endl;
            } else if (input == "validate" || input == "va") {
                
            } else if (input == "exit" || input == "e") {
                std::cout << "Exiting the program." << std::endl;
                exit(0); 
            } else if (input == "view" || input == "v") {
                std::cout << "What would you like to view?: ";
                std::cin >> input;
                if (input == "solution" || input == "s") {
                    if (numSolutions == 0) {
                        std::cout << "Looks like the single-threaded approach didn't find any solutions! Maybe you ran multi-threaded?" << std::endl;
                        continue;
                    }
                    std::cout << "Which solution would you like to see? [0-" << std::to_string(numSolutions - 1) << "]: ";
                    int solutionIndex;
                    if (!(std::cin >> solutionIndex)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cerr << msgs::invalidIntegerInputSolutionsIndexErrorMessage << std::endl;
                        continue;
                    } else if (solutionIndex < 0 || solutionIndex > numSolutions - 1) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cerr << msgs::invalidIntegerInputSolutionsIndexErrorMessage << std::endl;
                        continue;
                    }
                    const Solution& solution = solutionsVector[solutionIndex];
                    dsply::displaySolution(solution, pieceList);
                    if (boardsPopulatedVector[solutionIndex] == 0) {
                        boardsVector[solutionIndex] = solutions.constructBoardFromSolution(solution, pieceList);
                        boardsPopulatedVector[solutionIndex] = 1;
                    }
                    dsply::displayBoard(&(boardsVector[solutionIndex]));
                } else if (input == "threadsolution" || input == "ts") {
                    if (thread_numSolutions == 0) {
                        std::cout << "Looks like the multi-threaded approach didn't find any solutions! Maybe you ran single-threaded?" << std::endl;
                        continue;
                    }
                    std::cout << "Which solution would you like to see? [0-" << std::to_string(thread_numSolutions - 1) << "]: ";
                    int solutionIndex;
                    if (!(std::cin >> solutionIndex)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cerr << msgs::invalidIntegerInputSolutionsIndexErrorMessage << std::endl;
                        continue;
                    } else if (solutionIndex < 0 || solutionIndex > thread_numSolutions - 1) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cerr << msgs::invalidIntegerInputSolutionsIndexErrorMessage << std::endl;
                        continue;
                    }
                    const Solution& solution = thread_solutionsVector[solutionIndex];
                    dsply::displaySolution(solution, pieceList);
                    if (thread_boardsPopulatedVector[solutionIndex] == 0) {
                        thread_boardsVector[solutionIndex] = thread_solutions.constructBoardFromSolution(solution, pieceList);
                        thread_boardsPopulatedVector[solutionIndex] = 1;
                    }
                    dsply::displayBoard(&(thread_boardsVector[solutionIndex]));
                }
                
                else if (input == "all" || input == "a") {

                } else if (input == "numsolutions" || input == "ns") {
                    std::cout << "Solutions: " << std::to_string(numSolutions) << "\n";
                } else if (input == "piecelist" || input == "pl") {
                    dsply::displayPieceList(pieceList);
                } else {
                    std::cout << msgs::unrecognizedCommandErrorMessage(input) << std::endl;
                }
            } else {
                std::cout << msgs::unrecognizedCommandErrorMessage(input) << std::endl;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << msgs::invalidStringInputErrorMessage << std::endl;
            //std::cout << "Sorry, looks like that command failed completely. Please enter a valid command." << std::endl;
        }
    }
}

/*
void acceptUserInputAndDisplayCorrespondingInformation(somehow get access to the Solutions) {
    std::cout << msgs::stage4IntroMessage << std::endl;
    while (true) {
        get user input
        if input is show 1st solution:
            show first solution


    }

    //shell to get user input for display

    //use display class accordingly

    
    


    
//}*/
