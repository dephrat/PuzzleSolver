#ifndef SOLUTIONS_HPP
#define SOLUTIONS_HPP

#include <iostream> //latest thing maybe dont need, probably will remove

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
            if (maxSolutions == -1) {
                solutions.insert(solutions.end(), solns.begin(), solns.end());
                numSolutions += solns.size();
            } else {
                size_t numToAdd = maxSolutions - numSolutions;
                if (numToAdd < 0) {
                    std::cerr << "what the fuck\n";
                    return;
                } else if (numToAdd >= solns.size()) {
                    solutions.insert(solutions.end(), solns.begin(), solns.end());
                    numSolutions += solns.size();
                } else {
                    solutions.insert(solutions.end(), solns.begin(), solns.begin() + numToAdd);
                    numSolutions += numToAdd;
                }
            }
        }

        Board constructBoardFromSolution(const Solution& solution, const std::vector<std::string>& pieceList) const;
};

#endif