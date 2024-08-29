#include <iostream>

#include "../include/display.hpp"
#include "../include/messages.hpp"
#include "../include/piece.hpp"
#include "../include/stage1.hpp"

bool isValidPieceName(const std::string& name) {
    return pcs::piece_names.find(name) != pcs::piece_names.end();
}

std::vector<std::string> shellCreatePieceList() {
    std::cout << msgs::stage1IntroMessage << std::endl;
    std::vector<std::string> pieceList;
    while (true) {
        std::string input;
        std::cout << "Your command: ";
        if (std::cin >> input) {
            if (input == "help" || input == "h") {
                std::cout << msgs::stage1CommandsMessage << std::endl;
            } else if (input == "view" || input == "v") {
                std::cout << "What would you like to view?: ";
                std::cin >> input;
                if (input == "piece") {
                    std::cout << "Which piece?: ";
                    if (std::cin >> input) {
                        if (isValidPieceName(input)) {
                            dsply::displayPiece(pcs::piece_names.at(input));
                        } else {
                            std::cerr << msgs::unrecognizedPieceErrorMessage(input) << std::endl;
                        }
                    } else {
                        std::cin.clear();
                        std::cerr << msgs::invalidStringInputErrorMessage << std::endl;
                    }
                } else if (input == "pieces") {
                    dsply::displayAllPieces();
                    std::cout << "View each piece's info? Y/n: ";
                    std::cin >> input;
                    if (input == "Y")
                        dsply::displayAllPieces(true);
                } else if (input == "piecelist") {
                    dsply::displayPieceList(pieceList);
                    std::cout << "View each piece's info? Y/n: ";
                    std::cin >> input;
                    if (input == "Y") 
                        dsply::displayPieceList(pieceList, true);
                } else {
                    std::cout << msgs::unrecognizedCommandErrorMessage(input) << std::endl;
                }
            } else if (input == "create" || input == "cr") { // I could add the option to 'add' to your previous list if it isn't empty instead of starting from scratch, but it's so short that I don't think it'll be worth it
                int numPieces;
                std::cout << "How many pieces in your piece list?: ";
                if (!(std::cin >> numPieces)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << msgs::invalidIntegerInputErrorMessage << std::endl;
                    continue;
                } else if (numPieces < 0) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << msgs::invalidIntegerInputErrorMessage << std::endl;
                    continue;
                }
                pieceList.clear();
                for (int i = 0; i < numPieces; i++) {
                    std::cout << "Piece #" << std::to_string(i+1) << ": ";
                    if (!(std::cin >> input)) {
                        std::cin.clear();
                        std::cerr << msgs::invalidStringInputErrorMessage << std::endl;
                        break;
                    } else if (!isValidPieceName(input)) {
                        std::cerr << msgs::unrecognizedPieceErrorMessage(input) << std::endl;
                        break;
                    }
                    pieceList.push_back(input);
                }
            } else if (input == "createoriginal" || input == "cro") {
                pieceList = {"red", "blue", "orange", "green", "yellow", "cyan", "lime", "pink", "purple"};
                std::cout << "Using the original piece list\n";
            } else if (input == "continue" || input == "co") {
                std::cout << "Confirm the following piece list:\n";
                for (std::string s : pieceList) {
                    std::cout << s << "\n";
                }
                std::cout << "Y/n?: ";
                if (!(std::cin >> input)) {
                    std::cin.clear();
                    std::cerr << msgs::invalidStringInputErrorMessage << std::endl;
                } else if (input == "Y") {
                    std::cout << "Proceeding to Stage 2." << std::endl;
                    return pieceList;
                } else {
                    std::cout << "Cancelling." << std::endl;
                }
            } else if (input == "exit" || input == "e") {
                std::cout << "Exiting the program." << std::endl;
                exit(0); 
            } else {
                std::cout << msgs::unrecognizedCommandErrorMessage(input) << std::endl;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << msgs::invalidStringInputErrorMessage << std::endl;
        }
    }
}

void sortPieceList(std::vector<std::string>& pieceList) {
    std::sort(pieceList.begin(), pieceList.end(), [](const std::string& a, const std::string& b){
        const auto& bb1 = pcs::piece_names.at(a)->orientations[0].boundingBox;
        int size1 = (bb1.down - bb1.up) * (bb1.right - bb1.left);
        const auto& bb2 = pcs::piece_names.at(b)->orientations[0].boundingBox;
        int size2 = (bb2.down - bb2.up) * (bb2.right - bb2.left);
        return size1 > size2;
    });
}