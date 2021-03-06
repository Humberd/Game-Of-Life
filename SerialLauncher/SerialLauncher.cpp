#include "stdafx.h"
#include <chrono>
#include "file_manager.h"
#include "worker.h"
#include "boardGenerator.h"
#include "board_utils.h"
#include "globals.h"

int main(int argc, char* argv[]) {
    int numberOfIterations = 0;
    int boardSize = 0;
    std::string boardInitPattern;
    if (argc < 4) {
        printf("Program requires at least 3 arguments:\n");
        printf("1. boardSize: int - matrix size\n");
        printf("2. numberOfIterations: int - number of iterations\n");
        printf("3. boardInitPattern: string - allowed values = 'random', 'verticalStripes'\n");
        printf("4. (optional) saveImages: bool - should save images after each iteration\n");
        printf("5. (optional) debugEnabled: bool - should enable debug logging\n");
        return 1;
    }

    boardSize = atoi(argv[1]);
    numberOfIterations = atoi(argv[2]);
    boardInitPattern = argv[3];
    if (boardInitPattern != "random" &&
        boardInitPattern != "verticalStripes") {
        printf("boardInitPattern parameter must be either 'random' or 'verticalStripes'");
        return 1;
    }


    if (argc >= 5) {
        std::string argSaveImages(argv[4]);
        if (argSaveImages == "true") {
            saveImages = true;
        } else {
            saveImages = false;
        }
    }
    if (argc >= 6) {
        std::string argDebugEnabled(argv[5]);
        if (argDebugEnabled == "true") {
            debugEnabled = true;
        } else {
            debugEnabled = false;
        }
    }

    printf("====================\n");
    printf("Initiating program with:\n");
    printf("boardSize = %d\n", boardSize);
    printf("numberOfIterations = %d\n", numberOfIterations);
    printf("boardInitPattern = %s\n", boardInitPattern.c_str());
    printf("saveImages = %hhd\n", saveImages);
    printf("debugEnabled = %hhd\n", debugEnabled);
    printf("====================\n");

    /*-----------------------------------------------------------------------*/

    bool** board;
    if (boardInitPattern == "verticalStripes") {
        board = verticalStripesBoard(boardSize);
    }
    else if (boardInitPattern == "random") {
        board = randomBoard(boardSize);
    }
    else {
        printf("Unknown boardInitPattern: %s", boardInitPattern.c_str());
        return 1;
    }
    bool* virtualColumn = getVirtualColumn(boardSize);

    saveBoard(board, boardSize, -1);

    auto start = std::chrono::steady_clock::now();
    for (int i = 1; i <= numberOfIterations; i++) {
        bool** newBoard = serialIteration(board, boardSize, i, virtualColumn);
        cleanupBoard(board, boardSize);
        board = newBoard;
    }

    cleanupBoard(board, boardSize);
    delete[] virtualColumn;

    const auto end = std::chrono::steady_clock::now();
    const auto diff = end - start;


    printf("--ENDED %d iterations in total of %fs--\n", numberOfIterations,
           std::chrono::duration<double, std::milli>(diff / 1000).count());

    return 0;
}
