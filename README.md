#This is a multithreaded puzzle solver in which I implement thread pools to improve the speed.

##Overview
This project was created to find solutions to a puzzle, in which 9 pieces have to be placed into a 10x10 grid. I implemented both single- and multi-threaded solving with a thread pool, as well as some barebones benchmarking to compare them.

##Build & Run:
- `cd` into the repo
- `make`
- `./puzzleSolver`
- Answer the command-line prompts. The general flow is `create [num_pieces] [piece colours] continue Y [numthreads] [batch_size] [num_solutions]`, and you can use `help` to see other commands

##Features: List of key features
- Create list of pieces to be placed in the board
- Solver which allows single- and multi-threaded solving
- Display solutions after solving is completed
