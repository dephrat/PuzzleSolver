## Puzzle Solver

### Overview
This is a puzzle solver!  
The project was created to find solutions to a puzzle: Place 9 pieces into a 10x10 grid. I came up with single- and multi-threaded approaches, and you can use both to see which one runs faster.
The single-threaded approach used recursive backtracking, while the multi-threaded approach split the challenge into various Tasks and distributed them to a thread pool.

### Build & Run:
- `cd` into the repo
- `make`
- `./puzzleSolver`
- Answer the command-line prompts. The general flow is `create [num_pieces] [piece colours] continue Y [numthreads] [batch_size] [num_solutions]`, and you can use `help` to see other commands

### Features:
- Shell to create custom piece list!
- Solver with multi-threaded capabilities, implemented using a thread pool!
- Benchmarking!
- Post-solver shell to view solutions!

### Notes:
- The performance improvement seen by multithreading scales linearly with the number of CPU cores you have access to. The more cores, the better!
- I found 100 threads and a batch size of 1000 to be pretty consistently fast
- use `createoriginal` or `cro` in stage 1 to automatically set the piece list to that of the original puzzle
