#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <array>
#include <utility>
#include <vector>

#include "piece.hpp"
#include "task.hpp"

using Location = std::pair<int, int>;
using Solution = std::vector<std::pair<Location, int>>; //pair<Location, orientation index>, can reconstruct a game state using this and a Board
using Board = std::array<char, 10 * 10>;

class Solutions;

namespace slvr {

    class Solver {
    private:

        const std::vector<std::string> pieceList;
        Solutions& solutions;
        Task task;
        std::vector<Location> locations;

        void prepBoard();
        void prepLocations();
        std::vector<int> checkPlacement(const pcs::Piece* piece, const Location& location, const Board* board);
        

        //Board newBoard() {
            //this one gives you a new board, will probably be used for threads
        //}

    public:

        static void placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task);
        static void placePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Board* board);
        static void removePiece(const pcs::Piece* piece, int orientation_idx, const Location& location, Task* task);

        //Solver(const std::vector<std::string>& pieceList, int maxSolutions = 1); Changed Solver's solutions to a reference
        Solver(const std::vector<std::string>& pieceList, Solutions& solutions);
        void solve(const int pc_idx = 0);
    };
}

#endif
