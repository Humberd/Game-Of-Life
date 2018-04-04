#include "stdafx.h"
#include "Master.h"
#include <mpi.h>
#include "defs.h"
#include "mpi_helpers.h"
#include "boardGenerator.h"
#include "board_utils.h"
#include "file_manager.h"
#include <algorithm>
#include "globals.h"

int getDestSlave(int columnIndex, int threads) {
    return (columnIndex % (threads - 1)) + 1;
}

void Master::sendColumnsInCharge() {
    bool** startingBoard = verticalStripesBoard(boardSize);

    /* Populating every process with columns they are responsible for */
    for (int i = 0; i < boardSize; i++) {
        BoardColumn bc;
        bc.iteration = INIT_COLUMNS_ITERATION;
        bc.column = startingBoard[i];
        bc.columnIndex = i;
        if (i == 0) {
            bc.leftColumnProcRank = NO_SIDE_COLUM_PROC_RANK;
        } else {
            bc.leftColumnProcRank = getDestSlave(i - 1, threads);
        }

        if (i == boardSize - 1) {
            bc.rightColumnProcRank = NO_SIDE_COLUM_PROC_RANK;
        } else {
            bc.rightColumnProcRank = getDestSlave(i + 1, threads);
        }


        int destSlave = getDestSlave(i, threads);
        send(bc, destSlave, INIT_COLUMNS_PHASE_TAG, MPI_COMM_WORLD, boardSize);

        processesInCharge[i] = destSlave;
    }
    saveBoard(startingBoard, boardSize, -1);
    cleanupBoard(startingBoard, boardSize);

    /* Informing every thread that board initialization has been completed */
    for (int i = 1; i < threads; ++i) {
        BoardColumn bc;
        bc.iteration = INIT_COLUMNS_ITERATION;
        bc.column = NULL;
        bc.columnIndex = INIT_COLUMNS_PHASE_ENDED;
        bc.leftColumnProcRank = NO_SIDE_COLUM_PROC_RANK;
        bc.rightColumnProcRank = NO_SIDE_COLUM_PROC_RANK;

        send(bc, i, INIT_COLUMNS_PHASE_TAG, MPI_COMM_WORLD, 0);
    }
}

void Master::recvColumnsToSave(int iteration) {
    bool** board = new bool*[boardSize];
    for (int i = 0; i < boardSize; ++i) {
        BoardColumn bc;
        board[i] = new bool[boardSize];
        recv(bc, processesInCharge[i], SAVE_BOARD_PHASE_TAG, MPI_COMM_WORLD, boardSize);
        board[i] = bc.column;
    }
    if (saveImages) {
        saveBoard(board, boardSize, iteration);
    }

    cleanupBoard(board, boardSize);
}
