#pragma once
#include "mpi_helpers.h"
#include <vector>

class Slave {
public:
    int rank;
    int boardSize;
    std::vector<BoardColumn> bcs; // BoardColumnS

    Slave(int rank, int boardSize) : rank(rank),
                                     boardSize(boardSize) {
    }

    ~Slave();

    void recvColumnsInCharge();
};
