#ifndef SOLUTIONS_HPP
#define SOLUTIONS_HPP

#include <array>
#include <utility>
#include <vector>

namespace pcs {
    struct Piece;
}
struct Task;

using Location = std::pair<int, int>;
using Solution = std::vector<std::pair<Location, int>>; //pair<Location, orientation index>, can reconstruct a game state using this and an empty Board
using Board = std::array<char, 10 * 10>;

namespace slvr {
    class Solver; // Forward declaration
}

class Solutions { 
    private:
        std::vector<Solution> solutions;
        const int maxSolutions; //MIL stands for?
        int numSolutions = 0;

    public:
        Solutions(int maxSolns = 1) : maxSolutions(maxSolns) {}

        const std::vector<Solution>& getSolutions() const { return solutions; }
        int getNumSolutions() const { return numSolutions; }
        int getMaxSolutions() { return maxSolutions; }
        
        bool maxSolutionsReached() { return numSolutions >= maxSolutions && maxSolutions >= 0; }
        
        void addSolution(Solution& solution) { 
            solutions.push_back(solution);
            ++numSolutions;
        }

        //doesn't modify Solutions
        Board constructBoardFromSolution(const Solution& solution, const std::vector<std::string>& pieceList) const;

        //IDEA: WE COULD PLACE COMPLETED TASKS IN A SEPARATE QUEUE, AND HAVE A SINGLE THREAD CONSTANTLY RESPONSIBLE
        //FOR ADDING THOSE TASKS TO SOLUTIONS
        //THEREBY AVOIDING THE NEED FOR MULTITHREADED PROTECTION
        //the queue would still need multithreaded protection... I will reexamine this after designing the task queue

};

#endif