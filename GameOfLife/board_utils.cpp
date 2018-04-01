#include "stdafx.h"
#include "board_utils.h"
#include <algorithm>


/**
* Returns a column that has all values set to false
*/
bool* getVirtualColumn(int boardSize) {
    bool* newColumn = new bool[boardSize];
    std::fill_n(newColumn, boardSize, false);

    return newColumn;
}

void cleanupBoard(bool** board, int boardSize) {
    for (int j = 0; j < boardSize; j++) {
        delete[] board[j];
    }
    delete[] board;
}