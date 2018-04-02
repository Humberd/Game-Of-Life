#include "stdafx.h"
#include "Slave.h"
#include <mpi.h>
#include "mpi_helpers.h"
#include "defs.h"


void Slave::getColumnsInCharge() {
    for (;;) {
        BoardColumn bc;
        recv(bc, MASTER_RANK, INIT_COLUMNS_TAG, MPI_COMM_WORLD, boardSize);
        if (bc.columnIndex == INIT_COLUMNS_PHASE_ENDED) {
            break;
        }
        bcs.push_back(bc);
    }
    if (bcs.empty()) {
        printf("Slave %d received no columns to be in charge of. Terminating peacefully...", rank);
        return;
    }

    printf("Slave %d is in charge of %d columns: [", rank, bcs.size());
    for (auto boardColumn : bcs) {
        printf("%d,", boardColumn.columnIndex);
    }
    printf("]\n");
}
