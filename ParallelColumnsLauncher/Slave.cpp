#include "stdafx.h"
#include "Slave.h"
#include <mpi.h>
#include "mpi_helpers.h"
#include "defs.h"
#include "board_utils.h"
#include "worker.h"

Slave::~Slave() {
    for (auto bc : bcs) {
        delete[] bc.column;
    }

    delete[] virtualColumn;
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
    for (auto& boardColumn : bcs) {
        if (&boardColumn != &bcs.back()) {
            printf("%d,", boardColumn.columnIndex);
        } else {
            printf("%d", boardColumn.columnIndex);
        }

    }
    printf("]\n");
}

void Slave::iteration() {
    /* Sending columns to neighbours */
    std::vector<MPI_Request*> requests;
    for (int i = 0; i < bcs.size(); ++i) {
        if (bcs[i].leftColumnProcRank != NO_SIDE_COLUM_PROC_RANK) {
            printf("Slave %d has a left neighbour. Sending a column\n", rank);
            MPI_Request* reqs;
            reqs = sendAsync(bcs[i], bcs[i].leftColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            requests.push_back(&reqs[0]);
            requests.push_back(&reqs[1]);
            delete[] reqs;
        }
        if (bcs[i].rightColumnProcRank != NO_SIDE_COLUM_PROC_RANK) {
            printf("Slave %d has a right neighbour. Sending a column\n", rank);
            MPI_Request* reqs;
            reqs = sendAsync(bcs[i], bcs[i].rightColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            requests.push_back(&reqs[0]);
            requests.push_back(&reqs[1]);
            delete[] reqs;
        }
    }

    /* Receiving columns from neighbours */
    bool *leftColumn, *rightColumn;
    for (auto& bc : bcs) {
        if (bc.leftColumnProcRank == NO_SIDE_COLUM_PROC_RANK) {
            leftColumn = virtualColumn;
        } else {
            BoardColumn leftbc;
            recv(leftbc, bc.leftColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            leftColumn = leftbc.column;
        }

        if (bc.rightColumnProcRank == NO_SIDE_COLUM_PROC_RANK) {
            rightColumn = virtualColumn;
        } else {
            BoardColumn rightbc;
            recv(rightbc, bc.rightColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            rightColumn = rightbc.column;
        }

        bool* newColumn = generateNewColumn(leftColumn, bc.column, rightColumn, boardSize);

        BoardColumn newbc = bc;
        newbc.column = newColumn;
        newBcs.push_back(newbc);
    }
}

bool* Slave::getColumn(int columnIndex) {
    for (auto& bc : bcs) {
        if (bc.columnIndex == columnIndex) {
            return bc.column;
        }
    }

    return nullptr;
}
