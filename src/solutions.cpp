#include "../include/solutions.hpp"
#include "../include/solver.hpp"

#include "../include/piece.hpp"


Board Solutions::constructBoardFromSolution(const Solution& solution, const std::vector<std::string>& pieceList) const {
    Board board;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            board[10 * row + col] = ' ';
        }
    }

    for (int i = 0; i < pieceList.size(); ++i) {
        slvr::Solver::placePiece(pcs::piece_names.at(pieceList[i]), solution[i].second, solution[i].first, &board);
    }

    return board;
}