#pragma once

int getWorkers();

void generateNewColumn(bool* oldLeft, bool* oldCenter, bool* oldRight, unsigned int size);

short countLivingCells(const bool* left3, const bool* center3, const bool* right3);

short countLivingCellsBottomEdge(const bool* left2, const bool* center2, const bool* right2);

short countLivingCellsTopEdge(const bool* left2, const bool* center2, const bool* right2);