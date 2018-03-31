#include "stdafx.h"
#include "worker.h"

int getWorkers() {
    return 5;
}


void generateNewColumn(bool* oldLeft, bool* oldCenter, bool* oldRight, const unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {

    }
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
    livingCells -= center2[1];

    return livingCells;
}

short countLivingCellsTopEdge(const bool* left2, const bool* center2, const bool* right2) {
    short livingCells = 0;

    for (short i = 0; i < 2; i++) {
        livingCells += left2[i] + center2[i] + right2[i];
    }

    /* Subtract the cell in the center */
    livingCells -= center2[0];

    return livingCells;
}
