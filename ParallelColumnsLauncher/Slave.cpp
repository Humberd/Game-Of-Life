#include "stdafx.h"
#include "Slave.h"
#include <mpi.h>
#include "mpi_helpers.h"
#include "defs.h"

Slave::~Slave() {
    for (auto bc : bcs) {
        delete[] bc.column;
    }
}

void Slave::recvColumnsInCharge() {
    for (;;) {
        BoardColumn bc;
        recv(bc, MASTER_RANK, INIT_COLUMNS_TAG, MPI_COMM_WORLD, boardSize);
        if (bc.columnIndex == INIT_COLUMNS_PHASE_ENDED) {
            break;
        }
        bcs.push_back(bc);
    }
    if (bcs.empty()) {
        return;
    }

    printf("Slave %d is in charge of %d columns: [", rank, bcs.size());
    for (auto boardColumn : bcs) {
        printf("%d,", boardColumn.columnIndex);
    }
    printf("]\n");
}
