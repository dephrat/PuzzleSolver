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
void shellDisplayResults(const Solutions& solutions, const std::vector<std::string>& pieceList) {
    std::cout << msgs::stage4IntroMessage << std::endl;

    int numSolutions = solutions.getNumSolutions();
    const std::vector<Solution>& solutionsVector = solutions.getSolutions();
    std::vector<Board> boardsVector(numSolutions);
    std::vector<int> boardsPopulatedVector(numSolutions, 0);

    if (numSolutions == 0) {
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
                } else if (input == "all" || input == "a") {

                } else if (input == "numsolutions" || input == "ns") {
                    std::cout << "Solutions: " << std::to_string(numSolutions) << "\n";
                } else if (input == "piecelist" || input == "pl") {
                    
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
