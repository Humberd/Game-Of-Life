#pragma once
#include "mpi_helpers.h"
#include <vector>
#include "board_utils.h"

class Slave {
public:
    int rank;
    int boardSize;
    std::vector<BoardColumn> bcs; // BoardColumnS
    bool* virtualColumn;

    Slave(int rank, int boardSize) : rank(rank),
                                     boardSize(boardSize) {
        virtualColumn = getVirtualColumn(boardSize);
    }

    ~Slave();

    void recvColumnsInCharge();

    void iteration(int iteration);

    void saveColumns(int iteration);

    bool* getColumn(int columnIndex);
};
