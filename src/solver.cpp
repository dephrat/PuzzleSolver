#include "../include/solver.hpp"

#include "../include/solutions.hpp"

namespace slvr {

    void Solver::prepBoard() {
        for (int i = 0; i < 10 * 10; ++i) {
            task.board[i] = ' ';
        }
    }

    void Solver::prepLocations() {
        for (int i = 0; i < 10; i++) 
            for (int j = 0; j < 10; j++)
                locations.push_back(std::make_pair(i, j));
    }

    std::vector<int> Solver::checkPlacement(const pcs::Piece* piece, const Location& location, const Board* board) {
        //return true for the orientations that would fit at current Location

        std::vector<int> ret;
        unsigned char i = 0;
        bool placeable;
        for (const pcs::Orientation& orientation : piece->orientations) {
            placeable = true;
            for (const Location& coordinate : orientation.coordinates) {
                int row = coordinate.first + location.first;
                int col = coordinate.second + location.second;
                if (row < 0 || row >= 10 || col < 0 || col >= 10) { //maybe similar optimization here as in placePiece()?
                    placeable = false;
                    break;
                }
                if ((*board)[row * 10 + col] != ' ') { //which would likely affect this as well
                    placeable = false;
                    break;
                }//note that these previous two conditions could be merged bc they do the same thing - just keeping them separate for now
                //because of the possibility of future optimization, in which case I want it to be easily visible
            }
            if (placeable) ret.push_back(i);
            i++;
        }
        return ret;
    }

    //Modifies the board!
    //Please make sure you verify checkPlacement for this particular orientation
    void Solver::placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Board* board) {
        int idx_offset = 10 * location.first + location.second;
        for (const Location& coordinate : piece->orientations[orientation_idx].coordinates) {
            //int row = coordinate.first + location.first;
            //int col = coordinate.second + location.second;
            //int idx = row * 10 + col; // = (coordinate.first + location.first) * 10 + coordinate.second + location.second
            int idx = 10 * coordinate.first + coordinate.second + idx_offset;
            (*board)[idx] = piece->symbol;
        }
    }

    void Solver::placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task) {
        int idx_offset = 10 * location.first + location.second;
        for (const Location& coordinate : piece->orientations[orientation_idx].coordinates) {
            int idx = 10 * coordinate.first + coordinate.second + idx_offset;
            task->board[idx] = piece->symbol;
        }
        task->current_state.push_back(std::make_pair(location, orientation_idx));
    }

    void Solver::removePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task) {
        //assert(task->current_state.back().first == location && task->current_state.back().second == orientation_idx)
        //maybe include the above line in a debugging mode

        int idx_offset = 10 * location.first + location.second;

        for (const Location& coordinate : piece->orientations[orientation_idx].coordinates) {
            //int row = coordinate.first + location.first;
            //int col = coordinate.second + location.second;
            //int idx = row * 10 + col; // = (coordinate.first + location.first) * 10 + coordinate.second + location.second
            int idx = 10 * coordinate.first + coordinate.second + idx_offset;
            task->board[idx] = ' ';
        }
        task->current_state.pop_back();
    }

    Solver::Solver(const std::vector<std::string>& pieceList, Solutions& solutions) : pieceList(pieceList), solutions(solutions) {
        prepBoard();
        prepLocations();
    }

    void Solver::solve(const int pc_idx) { //default value already given in header file, are we allowed it here, and will it give an error?
        const pcs::Piece* piece = pcs::piece_names.at(pieceList[pc_idx]);
        for (const Location& location : locations) {
            
            std::vector<int> result = checkPlacement(piece, location, &(task.board));

            for (int i : result) {
                placePiece(piece, i, location, &task);
                if (pc_idx < pieceList.size() - 1) {
                    solve(pc_idx + 1);
                } else {
                    solutions.addSolution(task.current_state);//in multithreaded solving, we would use the Task passed in by reference
                        //but for now we use the Solver's Task
                }
                if (solutions.maxSolutionsReached()) return;
                removePiece(piece, i, location, &task);
            }
        }
    }
}