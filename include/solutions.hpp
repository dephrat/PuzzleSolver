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
using Solution = std::vector<std::pair<Location, int>>;
using Board = std::array<char, 10 * 10>;

namespace slvr {
    class Solver;
}

class Solutions { 
    private:
        std::vector<Solution> solutions;
        const int maxSolutions;
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
        void addSolutions(std::vector<Solution>& solns) { 
            solutions.insert(solutions.end(), solns.begin(), solns.end());
            numSolutions += solns.size();
        }

        Board constructBoardFromSolution(const Solution& solution, const std::vector<std::string>& pieceList) const;
};

#endif