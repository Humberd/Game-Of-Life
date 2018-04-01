#include "stdafx.h"
#include "boardGenerator.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

bool** verticalStripesBoard(int boardSize) {
    bool** board = new bool*[boardSize];
    for (int i = 0; i < boardSize; i++) {
        bool defaultValue = i % 2;
        bool* newColumn = new bool[boardSize];
        std::fill_n(newColumn, boardSize, defaultValue);
        board[i] = newColumn;
    }

    return board;
}

bool** randomBoard(int boardSize) {
    srand(time(NULL));

    bool** board = new bool*[boardSize];
    for (int i = 0; i < boardSize; i++) {
        bool* newColumn = new bool[boardSize];
        for (int j = 0; j<boardSize; j++) {
            newColumn[j] = rand() % 3;
        }
        board[i] = newColumn;
    }

    return board;
}