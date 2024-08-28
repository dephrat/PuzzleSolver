#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <array>
#include <iostream>
#include <chrono>
#include <climits>
#include <string>
#include <utility>
#include <vector>

namespace pcs {
    struct Piece;
}
class Solutions;
struct Task;

using Board = std::array<char, 10 * 10>;
using Location = std::pair<int, int>;
using Solution = std::vector<std::pair<Location, int>>; //pair<Location, orientation index>, can reconstruct a game state using this and a Board

namespace dsply {

    void displayPiece(const pcs::Piece* piece, int orientation_idx = 0);

    void displayPieceList(const std::vector<std::string>& pieceList, bool allDetails = false);

    void displayAllPieces(bool allDetails = false);

    void displayBoard(const Board* board);

    void displayTask(const Task* task);

    void displaySolution(const Solution&, const std::vector<std::string>&);

    void displaySolutions(const Solutions&);

}

#endif