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
    } else if (orientation_idx < -1 || orientation_idx >= piece->orientations.size()) {
        std::cerr << msgs::outOfBoundsOrientationIndexDisplayPieceErrorMessage << std::endl;
        return;
    } else {
        std::cout << "Piece name: " << piece->name << std::endl;
        const auto& coordinates = piece->orientations[orientation_idx].coordinates;
        if (coordinates.size() == 0) return;

        std::vector<std::vector<int>> displayStructure1, displayStructure2; //maybe I add this to the Orientation class itself?

        int firstRow = coordinates[0].first;
        int lastRow = coordinates.back().first;

        int firstCol1 = INT_MAX, firstCol2 = INT_MAX;



        using Time = std::chrono::steady_clock;
        std::chrono::duration<double, std::milli> fp_ms;

        const auto start1 = Time::now();

        //first option: a row for each spatial row from firstRow to lastRow
        int coordinate_idx = 0;
        std::vector<int> currentVec1;
        for (int i = firstRow; i <= lastRow; ++i) {
            currentVec1.clear(); //could add some code to maintain capacity and avoid reallocations but it's so small
            //invariant: coordinate_idx is the index of the first coordinate that hasn't yet been added
            while (coordinate_idx < coordinates.size() && coordinates[coordinate_idx].first == i) {
                currentVec1.push_back(coordinates[coordinate_idx].second);
                coordinate_idx++;
                firstCol1 = std::min(firstCol1, coordinates[coordinate_idx].second);
            }
            displayStructure1.push_back(currentVec1);
            //if (coordinate_idx >= coordinates.size()) break; this happening should only ever coincide with i == lastRow, so the loop breaks anyways
        }
        //if (coordinate_idx != coordinates.size()) then this is an error; by now we should've added all of the coordinates exactly once. We should be able to catch this mistake visually, though it would be a bit trickier

        //Display displayStructure1
        for (int i = 0; i < displayStructure1.size(); ++i) {
            //invariant: colTracker is where we expect to place the next char in the current row if it's as far left as possible, given the previously placed pieces in this row (starting from firstCol)
            int colTracker = firstCol1;
            for (int j = 0; j < displayStructure1[i].size(); colTracker = displayStructure1[i][j] + 1, ++j) {
                std::cout << std::string(displayStructure1[i][j] - colTracker, ' ') + piece->symbol;
            }
            std::cout << "\n";
        }

        const auto end1 = Time::now();
        fp_ms = end1 - start1;
        std::cout << "Display piece 1 finished. Time taken (s): " << fp_ms.count()/1000 << std::endl;


        const auto start2 = Time::now();
        
        //second option: a row for each populated spatial row with the first element indicating the spatial row index
        int currentRow = coordinates[0].first;
        std::vector<int> currentVec2 = {currentRow};
        for (const auto& coordinate : coordinates) {
            firstCol2 = std::min(firstCol2, coordinate.second);
            if (coordinate.first != currentRow) {
                displayStructure2.push_back(currentVec2);
                currentVec2.clear(); //potential reserving capacity here but it's so small I doubt it'll have much effect
                currentRow = coordinate.first;
                currentVec2 = {currentRow};
            }
            currentVec2.push_back(coordinate.second);
        }
        displayStructure2.push_back(currentVec2);

        //Display displayStructure2
        for (int i = 0; i < displayStructure2.size(); ++i) {
            //invariant: colTracker is where we expect to place the next char in the current row if it's as far left as possible, given the previously placed pieces in this row (starting from firstCol)
            int colTracker = firstCol2;
            //j starts at 1 because the first element in the vector is reserved for the spatial row
            for (int j = 1; j < displayStructure2[i].size(); colTracker = displayStructure2[i][j] + 1, j++) {
                std::cout << std::string(displayStructure2[i][j] - colTracker, ' ') + piece->symbol;
            }
            std::cout << "\n";
            if (i < displayStructure2.size() - 1) { //i.e. if this isn't the last row
                int row_gap = displayStructure2[i+1][0] - displayStructure2[i][0];
                if (row_gap > 1) {
                    std::cout << std::string(row_gap - 1, '\n');
                }
            }
        }

        const auto end2 = Time::now();
        fp_ms = end2 - start2;
        std::cout << "Display piece 2 finished. Time taken (s): " << fp_ms.count()/1000 << std::endl;

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
    //assert(solution.size() == pieceList.size()); //this goes in validateSolutions
    for (int i = 0; i < pieceList.size(); ++i) {
        const auto& solutionElement = solution[i];
        //Piece #(i+1): red(r) placed at (x,y) in orientation z
        std::cout << "Piece #" << std::to_string(i+1) << ": " << pieceList[i] << "(" << pcs::piece_names.at(pieceList[i])->symbol << ")";
        std::cout << " placed at (" << std::to_string(solutionElement.first.first) << ", " << std::to_string(solutionElement.first.second);
        std::cout << ") in orientation " << std::to_string(solutionElement.second) << std::endl;

    }
}

void displaySolutions(const Solutions& solutions) {

}

}