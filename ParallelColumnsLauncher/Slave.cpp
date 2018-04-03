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
    std::vector<MPI_Request*> requests;
    /* Sending columns to neighbours */
    for (int i = 0; i < bcs.size(); ++i) {
        /* When this column has left neighbour and its not me (this process) */
        if (bcs[i].leftColumnProcRank != NO_SIDE_COLUM_PROC_RANK &&
            bcs[i].leftColumnProcRank != rank) {
            printf("Slave %d - Column %d has left neighbour Slave %d. Sending him my column...\n", 
                rank, bcs[i].columnIndex, bcs[i].leftColumnProcRank);
            MPI_Request* reqs;
            reqs = sendAsync(bcs[i], bcs[i].leftColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            requests.push_back(&reqs[0]);
            requests.push_back(&reqs[1]);
            delete[] reqs;
        }

        /* When this column has right neighbour and its not me (this process) */
        if (bcs[i].rightColumnProcRank != NO_SIDE_COLUM_PROC_RANK &&
            bcs[i].rightColumnProcRank != rank) {
            printf("Slave %d - Column %d has right neighbour Slave %d. Sending him my column...\n",
                rank, bcs[i].columnIndex, bcs[i].rightColumnProcRank);
            MPI_Request* reqs;
            reqs = sendAsync(bcs[i], bcs[i].rightColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            requests.push_back(&reqs[0]);
            requests.push_back(&reqs[1]);
            delete[] reqs;
        }
    }

    bool *leftColumn, *rightColumn;
    /* Receiving columns from neighbours */
    for (auto& bc : bcs) {
        /* When I (this process) handles left column */
        if (bc.leftColumnProcRank == rank) {
            printf("Slave %d - Column %d left neighbour is me. Getting it from my own resources.\n",
                rank, bc.columnIndex);
            leftColumn = getColumn(bc.columnIndex);

            /* When there is no left column */
        } else if (bc.leftColumnProcRank == NO_SIDE_COLUM_PROC_RANK) {
            printf("Slave %d - Column %d has no left neighbour. I'm virtualizing it.\n",
                rank, bc.columnIndex);
            leftColumn = virtualColumn;

        } else {
            printf("Slave %d - Column %d left column is in charge by Slave %d. Receiving...\n",
                rank, bc.columnIndex, bc.leftColumnProcRank);
            BoardColumn leftbc;
            recv(leftbc, bc.leftColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            leftColumn = leftbc.column;
        }

        /* When I (this process) handles right column */
        if (bc.rightColumnProcRank == rank) {
            printf("Slave %d - Column %d right neighbour is me. Getting it from my own resources.\n",
                rank, bc.columnIndex);
            rightColumn = getColumn(bc.columnIndex);

            /* When there is no right column */
        } else if (bc.rightColumnProcRank == NO_SIDE_COLUM_PROC_RANK) {
            printf("Slave %d - Column %d has no right neighbour. I'm virtualizing it.\n",
                rank, bc.columnIndex);
            rightColumn = virtualColumn;

        } else {
            printf("Slave %d - Column %d right column is in charge by Slave %d. Receiving...\n",
                rank, bc.columnIndex, bc.rightColumnProcRank);
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
