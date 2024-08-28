#ifndef TASK_HPP
#define TASK_HPP

#include <array>
#include <utility>
#include <vector>

using Board = std::array<char, 10 * 10>;
using Location = std::pair<int, int>;
using Solution = std::vector<std::pair<Location, int>>;

struct Task {
    Board board;
    Solution current_state;
};

#endif