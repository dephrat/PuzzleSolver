#include "../include/display.hpp"

#include "../include/messages.hpp"
#include "../include/piece.hpp"
#include "../include/task.hpp"

namespace dsply {

    //display piece name and layout
    void displayPiece(const pcs::Piece* piece, int orientation_idx) {
        if (orientation_idx == -1) {
            for (size_t i = 0; i < piece->orientations.size(); ++i) {
                displayPiece(piece, i);
            }
        } else if (orientation_idx < -1 || orientation_idx >= (int)piece->orientations.size()) {
            std::cerr << msgs::outOfBoundsOrientationIndexDisplayPieceErrorMessage << std::endl;
            return;
        } else {
            std::cout << "\nName: " << piece->name << std::endl;
            std::cout << "Symbol: " << piece->symbol << std::endl;
            const auto& coordinates = piece->orientations[orientation_idx].coordinates;
            if (coordinates.size() == 0) return;

            std::vector<std::vector<int>> displayStructure; //maybe I add this to the Orientation struct itself?
            
            //Have a row for each populated spatial row with the first element indicating the spatial row index
            int firstCol = INT_MAX;
            int currentRow = coordinates[0].first;
            std::vector<int> currentVec = {currentRow};
            for (const auto& coordinate : coordinates) {
                firstCol = std::min(firstCol, coordinate.second);
                if (coordinate.first != currentRow) {
                    displayStructure.push_back(currentVec);
                    currentVec.clear();
                    currentRow = coordinate.first;
                    currentVec = {currentRow};
                }
                currentVec.push_back(coordinate.second);
            }
            displayStructure.push_back(currentVec);

            //Display
            for (size_t i = 0; i < displayStructure.size(); ++i) {
                //invariant: colTracker is where we expect to place the next char in the current row if it's as far left as possible, given the previously placed pieces in this row (starting from firstCol)
                int colTracker = firstCol;
                //j starts at 1 because the first element in the vector is reserved for the spatial row
                for (size_t j = 1; j < displayStructure[i].size(); colTracker = displayStructure[i][j] + 1, j++) {
                    std::cout << std::string(displayStructure[i][j] - colTracker, ' ') + piece->symbol;
                }
                std::cout << "\n";
                if (i < displayStructure.size() - 1) { //i.e. if this isn't the last row
                    int row_gap = displayStructure[i+1][0] - displayStructure[i][0];
                    if (row_gap > 1) {
                        std::cout << std::string(row_gap - 1, '\n');
                    }
                }
            }
        }
    }

    void displayPieceList(const std::vector<std::string>& pieceList, bool allDetails) {
        for (const std::string& s : pieceList) {
            if (allDetails) dsply::displayPiece(pcs::piece_names.at(s));
            else std::cout << s << std::endl;
        }
    }

    void displayAllPieces(bool allDetails) {
        for (const std::pair<std::string, pcs::Piece*>& p : pcs::piece_names) {
            if (allDetails) dsply::displayPiece(p.second);
            else std::cout << p.first << std::endl;
        }
    }

    void displayBoard(const Board* board) {
        char c;
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                c = (*board)[10 * row + col];
                std::cout << ((c == ' ') ? '.' : c);
            }
            std::cout << "\n";
        }
    }

    void displayTask(const Task* task) {
        displayBoard(&task->board);
    }

    void displaySolution(const Solution& solution, const std::vector<std::string>& pieceList) {
        for (int i = 0; i < pieceList.size(); ++i) {
            const auto& solutionElement = solution[i];
            //Piece #(i+1): red(r) placed at (x,y) in orientation z
            std::cout << "Piece #" << std::to_string(i+1) << ": " << pieceList[i] << "(" << pcs::piece_names.at(pieceList[i])->symbol << ")";
            std::cout << " placed at (" << std::to_string(solutionElement.first.first) << ", " << std::to_string(solutionElement.first.second);
            std::cout << ") in orientation " << std::to_string(solutionElement.second) << std::endl;
        }
    }
}