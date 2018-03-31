#include "stdafx.h"
#include "worker.h"
#include <exception>

bool* generateNewColumn(bool* oldLeft, bool* oldCenter, bool* oldRight, const unsigned int height) {
    const auto newColumn = new bool[height];

    for (unsigned int i = 0; i < height; i++) {
        short neighbourCells;
        bool currentCell;
        if (i == 0) {
            neighbourCells = countLivingCellsBottomEdge(&oldLeft[0], &oldCenter[0], &oldRight[0]);
            currentCell = oldCenter[0];
        } else if (i == height - 1) {
            neighbourCells = countLivingCellsTopEdge(&oldLeft[height - 2], &oldCenter[height - 2], &oldRight[height - 2]);
            currentCell = oldCenter[height - 1];
        } else {
            neighbourCells = countLivingCells(&oldLeft[i - 1], &oldCenter[i - 1], &oldRight[i - 1]);
            currentCell = oldCenter[i];
        }

        newColumn[i] = decideNewCellState(&currentCell, &neighbourCells);
    }

    return newColumn;
}

bool decideNewCellState(const bool* currentCell, const short* neighbourCells) {
    if (*currentCell && (*neighbourCells == 2 || *neighbourCells == 3)) {
        return true;
    }
    if (*currentCell && *neighbourCells >= 4) {
        return false;
    }
    if (*currentCell && *neighbourCells < 2) {
        return false;
    }
    if (!*currentCell && *neighbourCells == 3) {
        return true;
    }

    return false;
}

short countLivingCells(const bool* left3, const bool* center3, const bool* right3) {
    short livingCells = 0;

    for (short i = 0; i < 3; i++) {
        livingCells += left3[i] + center3[i] + right3[i];
    }

    /* Subtract the cell in the center */
    livingCells -= center3[1];

    return livingCells;
}

short countLivingCellsBottomEdge(const bool* left2, const bool* center2, const bool* right2) {
    short livingCells = 0;

    for (short i = 0; i < 2; i++) {
        livingCells += left2[i] + center2[i] + right2[i];
    }

    /* Subtract the cell in the center */
    livingCells -= center2[0];

    return livingCells;
}

short countLivingCellsTopEdge(const bool* left2, const bool* center2, const bool* right2) {
    short livingCells = 0;

    for (short i = 0; i < 2; i++) {
        livingCells += left2[i] + center2[i] + right2[i];
    }

    /* Subtract the cell in the center */
    livingCells -= center2[1];

    return livingCells;
}
