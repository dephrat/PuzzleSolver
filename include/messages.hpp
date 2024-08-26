#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>

namespace msgs {

    extern const std::string illegalEmptyPieceSymbolErrorMessage;
    extern const std::string incorrectNumberPieceOrientationsErrorMessage;
    extern const std::string incorrectPieceNameErrorMessage;
    extern const std::string invalidIntegerInputBatchSizeErrorMessage;
    extern const std::string invalidIntegerInputErrorMessage;
    extern const std::string invalidIntegerInputNumSolutionsErrorMessage;
    extern const std::string invalidIntegerInputNumThreadsErrorMessage;
    extern const std::string invalidIntegerInputSolutionsIndexErrorMessage;
    extern const std::string invalidStringInputErrorMessage;
    extern const std::string nullPieceErrorMessage;
    extern const std::string outOfBoundsOrientationIndexDisplayPieceErrorMessage;
    extern const std::string repeatedPieceSymbolErrorMessage;
    extern const std::string stage1CommandsMessage;
    extern const std::string stage1IntroMessage;
    extern const std::string stage3CommandsMessage;
    extern const std::string stage3IntroMessage;

    extern std::string unrecognizedCommandErrorMessage(const std::string& command);
    extern std::string unrecognizedPieceErrorMessage(const std::string& pieceName);
    
}

#endif