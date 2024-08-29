## Puzzle Solver

### Overview
This project was created to find solutions to a puzzle, in which 9 pieces have to be placed into a 10x10 grid. I implemented both single- and multi-threaded solving with a thread pool, as well as some barebones benchmarking to compare the two approaches.

### Build & Run:
- `cd` into the repo
- `make`
- `./puzzleSolver`
- Answer the command-line prompts. The general flow is `create [num_pieces] [piece colours] continue Y [numthreads] [batch_size] [num_solutions]`, and you can use `help` to see other commands

### Features:
- Shell to create custom piece list!
- Solver with multi-threaded capabilities, implemented using a thread pool!
- Post-solver shell to view solutions!
