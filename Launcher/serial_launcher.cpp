#include "stdafx.h"
#include <chrono>
#include "file_manager.h"
#include "worker.h"
#include "boardGenerator.h"
#include "board_utils.h"

int launch_serial(int argc, char* argv[]) {
    const int numberOfIterations = 300;
    const int boardSize = 2048;
    bool** board = randomBoard(boardSize);
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
