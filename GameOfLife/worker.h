#pragma once

bool* generateNewColumn(bool* oldLeft, bool* oldCenter, bool* oldRight, const unsigned int size);

bool decideNewCellState(const bool* currentCell, const short* neighbourCells);

short countLivingCells(const bool* left3, const bool* center3, const bool* right3);

short countLivingCellsBottomEdge(const bool* left2, const bool* center2, const bool* right2);

short countLivingCellsTopEdge(const bool* left2, const bool* center2, const bool* right2);