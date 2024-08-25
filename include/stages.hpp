#ifndef STAGES_HPP
#define STAGES_HPP

//includes:

//Stage 0
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include "display.hpp"
#include "messages.hpp"
#include "piece.hpp"

//Stage 1
//#include <iostream>
//#include "messages.hpp"
//#include "piece.hpp"
#include "display.hpp"
#include <unordered_map>

//Stage 2
#include <chrono>
#include "solver.hpp"

////
//
//Stage 3

//Stage 4
using Solution = std::vector<std::pair<Location, int>>;


//functions:

//Stage 0
void stage0();

//Stage 1
std::vector<std::string> shellCreatePieceList();

//Stage 2
void shellPrepSolver(int&, int&, int&);
void runSolver(slvr::Solver&, const std::vector<std::string>&);

//Stage 3

//Stage 4
//void acceptUserInputAndDisplayCorrespondingInformation();
void shellDisplayResults(const slvr::Solver&, const std::vector<std::string>&);

#endif