#include "../include/solver.hpp"

#include "../include/solutions.hpp"

using Time = std::chrono::steady_clock;

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

    inline bool inBounds(const Location& location, const pcs::BB& bb) {
        int row = location.first, col = location.second;
        return bb.left + col >= 0 && bb.right + col < 10 && bb.up + row >= 0 && bb.down + row < 10;
    }
    
    std::vector<int> Solver::checkPlacement(const pcs::Piece* piece, const Location& location, const Board* board) {
        //return true for the orientations that would fit at current Location
        std::vector<int> ret;
        for (size_t i = 0; i < piece->orientations.size(); i++) {
            const pcs::Orientation& orientation = piece->orientations[i];
            if (!inBounds(location, orientation.boundingBox)) continue;
            bool placeable = true;
            for (const Location& coordinate : orientation.coordinates) {
                int row = coordinate.first + location.first;
                int col = coordinate.second + location.second;
                if ((*board)[row * 10 + col] != ' ') {
                    placeable = false;
                    break;
                }
            }
            if (placeable) ret.push_back(i);
        }
        return ret;
    }

    //Modifies the board!
    //Please make sure you verify checkPlacement for this particular orientation
    void Solver::placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Board* board) {
        int idx_offset = 10 * location.first + location.second;
        for (const Location& coordinate : piece->orientations[orientation_idx].coordinates) {
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
        int idx_offset = 10 * location.first + location.second;
        for (const Location& coordinate : piece->orientations[orientation_idx].coordinates) {
            int idx = 10 * coordinate.first + coordinate.second + idx_offset;
            task->board[idx] = ' ';
        }
        task->current_state.pop_back();
    }

    Solver::Solver(const std::vector<std::string>& pieceList, Solutions& solutions, Solutions& thread_solutions, int numThreads, 
    size_t batchSize) 
    : pieceList(pieceList), solutions(solutions), thread_solutions(thread_solutions), numThreads(numThreads), batchSize(batchSize),
    useBatching(batchSize > 0) {
        prepBoard();
        prepLocations();
    }

    void Solver::solve(const size_t pc_idx) {
        const pcs::Piece* piece = pcs::piece_names.at(pieceList[pc_idx]);
        for (const Location& location : locations) {
            std::vector<int> result = checkPlacement(piece, location, &(task.board));
            for (int i : result) {
                placePiece(piece, i, location, &task);
                if (pc_idx < -1 + pieceList.size())
                    solve(pc_idx + 1);
                else
                    solutions.addSolution(task.current_state);
                if (solutions.maxSolutionsReached()) return;
                removePiece(piece, i, location, &task);
            }
        }
    }
}