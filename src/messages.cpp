#include "../include/messages.hpp"

namespace msgs {
    
    const std::string incorrectNumberPieceOrientationsErrorMessage = "ERROR: Sorry, looks like one of the pieces was defined with more or less than one orientation. Check the documentation for more information on how to resolve this error.";
    const std::string incorrectPieceNameErrorMessage = "ERROR: Sorry, some of the internal piece_names keys don't match their mapped piece's name. Check the documentation for more information on how to resolve this error.";
    const std::string invalidIntegerInputBatchSizeErrorMessage = "ERROR: Sorry, please enter a valid non-negative integer less than 2147483648.";
    const std::string invalidIntegerInputErrorMessage = "ERROR: Sorry, please enter a valid non-negative integer less than 2147483648.";
    const std::string invalidIntegerInputNumSolutionsErrorMessage = "ERROR: Sorry, please enter either -1 or a valid non-negative integer less than 2147483648.";
    const std::string invalidIntegerInputNumThreadsErrorMessage = "ERROR: Sorry, please enter a valid non-negative integer less than 2147483648.";
    const std::string invalidIntegerInputSolutionsIndexErrorMessage = "ERROR: Sorry, please enter a valid integer within the specified range (inclusive).";
    const std::string invalidStringInputErrorMessage = "ERROR: Sorry, looks like that input failed completely. Please try again.";
    const std::string illegalEmptyPieceSymbolErrorMessage = "ERROR: Sorry, the space char (' ') cannot be used as a piece symbol because it represents an empty square in the board. Check the documentation for more information on how to resolve this error.";
    const std::string nullPieceErrorMessage = "ERROR: Sorry, looks like piece_names maps a string to a nullptr. Check the documentation for more information on how to resolve this error.";
    const std::string outOfBoundsOrientationIndexDisplayPieceErrorMessage = "ERROR: Sorry, looks like someone tried to call displayPiece with an illegal orientation index. Check the documentation for more information on how to resolve this error.";
    const std::string repeatedPieceSymbolErrorMessage = "ERROR: Sorry, some of the piece symbols were used for multiple pieces. Please ensure each piece uses a unique symbol. Check the documentation for more information on how to resolve this error.";
    const std::string stage1IntroMessage = "Stage 1: Use commands to put together a list of pieces. The solver will attempt to place these pieces into the grid. Enter 'help' for a list of accepted commands.";
    const std::string stage3IntroMessage = "Stage 3: Use commands to view the results. Enter 'help' for a list of accepted commands.";
    
    const std::string stage1CommandsMessage = std::string("\nhelp: lists all commands.\n") //update to include aliases, figure out how to stylize LOW PRIORITY
        +std::string("view: prompts for one of the three following follow-ups.\n")
        +std::string("   piece: prompts for piece name, then displays piece info.\n")
        +std::string("   pieces: lists all piece names, then asks if you'd like to display each piece's info.\n")
        +std::string("   piecelist: displays current piece list, which the solver will attempt to place in the puzzle grid,\n")
        +std::string("      then asks if you'd like to display each piece's info.\n")
        +std::string("create: prompts you to create the piece list to be used in the puzzle.\n")
        +std::string("continue: prompts to confirm the current piece list, then begins solving the puzzle.\n")
        +std::string("exit: exits the program.\n");

    const std::string stage3CommandsMessage = std::string("\nhelp: lists all commands.\n")
        +std::string("validate: performs a check that all solutions are valid.\n")
        +std::string("exit: exits the program.\n")
        +std::string("view: prompts for one of the following follow-ups.\n")
        +std::string("   solution: prompts for index, then shows the solution at that index.\n")
        +std::string("   threadsolution: prompts for index, then shows the multithreading-generated solution at that index.\n")
        +std::string("   all: prompts for confirmation, then shows all solutions.\n")
        +std::string("   numsolutions: outputs the number of solutions found.\n")
        +std::string("   numthreadsolutions: outputs the number of solutions found.\n")
        +std::string("   piecelist: displays the piece list, which the solver attempted to place in the puzzle grid,\n")
        +std::string("      then asks if you'd like to display each piece's info.\n");
        
    
    std::string unrecognizedCommandErrorMessage(const std::string& command) {
        return "ERROR: Sorry, " + command + " is not a recognized command."
        + " Enter 'help' for a list of valid commands.";
    }
    std::string unrecognizedPieceErrorMessage(const std::string& pieceName) {
        return "ERROR: Sorry, " + pieceName + " is not a recognized piece name."
        + " Enter 'view pieces' for a list of valid piece names,"
        + " or refer to the documentation to see how to add new pieces.";
    }
    
}