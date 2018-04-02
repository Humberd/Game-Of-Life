#pragma once
#include "mpi_helpers.h"
#include <vector>

class Slave {
private:
    int rank;
    int boardSize;
    std::vector<BoardColumn> bcs; // BoardColumnS
public:


    Slave(int rank, int boardSize) : rank(rank),
                                     boardSize(boardSize) {
    }

    ~Slave() = default;

    void getColumnsInCharge();
};
