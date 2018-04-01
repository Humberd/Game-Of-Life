#pragma once

/**
 * Returns a column that has all values set to false
 */
bool* getVirtualColumn(int boardSize);

void cleanupBoard(bool** board, int boardSize);
