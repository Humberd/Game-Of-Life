#include "stdafx.h"
#include "Slave.h"
#include <mpi.h>
#include "mpi_helpers.h"
#include "defs.h"
#include "board_utils.h"
#include "worker.h"
#include "Windows.h"
#include <algorithm>

Slave::~Slave() {
    for (auto bc : bcs) {
        delete[] bc.column;
    }

    delete[] virtualColumn;
}

void Slave::recvColumnsInCharge() {
    for (;;) {
        BoardColumn bc;
        recv(bc, MASTER_RANK, INIT_COLUMNS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
        if (bc.columnIndex == INIT_COLUMNS_PHASE_ENDED) {
            break;
        }
        bcs.push_back(bc);
    }
    if (bcs.empty()) {
        return;
    }

    printf_debug("Slave %d is in charge of %d columns: [", rank, bcs.size());
    for (auto& boardColumn : bcs) {
        if (&boardColumn != &bcs.back()) {
            printf_debug("%d,", boardColumn.columnIndex);
        } else {
            printf_debug("%d", boardColumn.columnIndex);
        }

    }
    printf_debug("]\n");
}

void Slave::iteration(int iteration) {
    std::vector<MPI_Request*> requests;
    std::vector<BoardColumn> newBcs;
    /* Sending columns to neighbours */
    for (int i = 0; i < bcs.size(); ++i) {
        /* When this column has left neighbour and its not me (this process) */
        if (bcs[i].leftColumnProcRank != NO_SIDE_COLUM_PROC_RANK &&
            bcs[i].leftColumnProcRank != rank) {
            printf_debug("Slave %d - Iteration %d - Column %d has left neighbour Slave %d. Sending him my column...\n",
                         rank, iteration, bcs[i].columnIndex, bcs[i].leftColumnProcRank);
            MPI_Request* reqs;
            // reqs = sendAsync(bcs[i], bcs[i].leftColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            // requests.push_back(&reqs[0]);
            // requests.push_back(&reqs[1]);
            // delete[] reqs;
        }

        /* When this column has right neighbour and its not me (this process) */
        if (bcs[i].rightColumnProcRank != NO_SIDE_COLUM_PROC_RANK &&
            bcs[i].rightColumnProcRank != rank) {
            printf_debug("Slave %d - Iteration %d - Column %d has right neighbour Slave %d. Sending him my column...\n",
                         rank, iteration, bcs[i].columnIndex, bcs[i].rightColumnProcRank);
            MPI_Request* reqs;
            // reqs = sendAsync(bcs[i], bcs[i].rightColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            // requests.push_back(&reqs[0]);
            // requests.push_back(&reqs[1]);
            // delete[] reqs;
        }
    }

    bool* leftColumn = nullptr;
    bool* rightColumn = nullptr;
    bool deleteLeftColumn = false;
    bool deleteRightColumn = false;
    /* Receiving columns from neighbours */
    for (auto& bc : bcs) {
        deleteLeftColumn = false;
        deleteRightColumn = false;
        /* When I (this process) handles left column */
        if (bc.leftColumnProcRank == rank) {
            printf_debug(
                "Slave %d - Iteration %d - Column %d left neighbour is me. Getting it from my own resources.\n",
                rank, iteration, bc.columnIndex);
            leftColumn = getColumn(bc.columnIndex);

            /* When there is no left column */
        } else if (bc.leftColumnProcRank == NO_SIDE_COLUM_PROC_RANK) {
            printf_debug("Slave %d - Iteration %d - Column %d has no left neighbour. I'm virtualizing it.\n",
                         rank, iteration, bc.columnIndex);
            leftColumn = virtualColumn;

        } else {
            printf_debug("Slave %d - Iteration %d - Column %d left column is in charge by Slave %d. Receiving...\n",
                         rank, iteration, bc.columnIndex, bc.leftColumnProcRank);
            BoardColumn leftbc;
            leftColumn = new bool[boardSize];
            // leftColumn = static_cast<bool*>(malloc(boardSize * sizeof(bool)));
            // recv(leftbc, bc.leftColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            // leftColumn = leftbc.column;
            deleteLeftColumn = true;
        }

        /* When I (this process) handles right column */
        if (bc.rightColumnProcRank == rank) {
            printf_debug(
                "Slave %d - Iteration %d - Column %d right neighbour is me. Getting it from my own resources.\n",
                rank, iteration, bc.columnIndex);
            rightColumn = getColumn(bc.columnIndex);

            /* When there is no right column */
        } else if (bc.rightColumnProcRank == NO_SIDE_COLUM_PROC_RANK) {
            printf_debug("Slave %d - Iteration %d - Column %d has no right neighbour. I'm virtualizing it.\n",
                         rank, iteration, bc.columnIndex);
            rightColumn = virtualColumn;

        } else {
            printf_debug("Slave %d - Iteration %d - Column %d right column is in charge by Slave %d. Receiving...\n",
                         rank, iteration, bc.columnIndex, bc.rightColumnProcRank);
            BoardColumn rightbc;
            rightColumn = new bool[boardSize];
            // rightColumn = static_cast<bool*>(malloc(boardSize * sizeof(bool)));
            // recv(rightbc, bc.rightColumnProcRank, ITERATIONS_PHASE_TAG, MPI_COMM_WORLD, boardSize);
            // rightColumn = rightbc.column;
            deleteRightColumn = true;
        }

        bool* newColumn = generateNewColumn(leftColumn, bc.column, rightColumn, boardSize);

        // free(leftColumn);
        // free(rightColumn);
        if (deleteLeftColumn) {
            delete[] leftColumn;
        }
        if (deleteRightColumn) {
            delete[] rightColumn;
        }
        // delete[] bc.column;
        BoardColumn newbc = bc;
        newbc.column = newColumn;
        newBcs.push_back(newbc);
        // printf("Sleeping \n");
        /* Await all the remaining requests */
        Sleep(1);

    }

    // MPI_Request** requestsArray = requests.data();
    // MPI_Status* statusesArray = new MPI_Status[requests.size()];
    // MPI_Waitall(requests.size(), *requestsArray, statusesArray);
    //
    for (auto& bc : bcs) {
        delete[] bc.column;
    }
    //
    bcs.clear();
    bcs = newBcs;
    newBcs.clear();
}

void Slave::saveColumns(int iteration) {
    for (auto& bc : bcs) {
        printf_debug("Slave %d - Iteration %d - Sending column %d to save...\n",
                     rank, iteration, bc.columnIndex);
        sendAsync(bc, MASTER_RANK, SAVE_BOARD_PHASE_TAG, MPI_COMM_WORLD, boardSize);
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
