#include "stdafx.h"
#include "Master.h"
#include <mpi.h>
#include "defs.h"
#include "mpi_helpers.h"
#include "boardGenerator.h"
#include "board_utils.h"
#include "file_manager.h"
#include <algorithm>

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
        printf("Master - to Slave %d [%d, %d, %d]\n", destSlave, bc.column[0], bc.column[1], bc.column[2]);
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
    //    bool** board = new bool*[boardSize];
    //    MPI_Request* reqs = new MPI_Request[boardSize];
    //    MPI_Status* statuses = new MPI_Status[boardSize];
    //    std::fill_n(statuses, boardSize, MPI_Status());
    //    for (int i = 0; i < boardSize; ++i) {
    //        board[i] = new bool[boardSize];
    //        MPI_Request* locReqs;
    //        locReqs = recvAsync(board[i], processesInCharge[i], SAVE_BOARD_PHASE_TAG, MPI_COMM_WORLD, boardSize);
    //
    //        reqs[i] = *locReqs;
    //    }
    //
    //    MPI_Waitall(boardSize, reqs, statuses);
    //    printf("Master - Iteration %d - Receiving %d columns to save.\n",
    //           iteration, boardSize);
    //
    ////    for (int i = 0; i < boardSize; ++i) {
    ////        int number_amount;
    ////        MPI_Get_count(&statuses[i], MPI_C_BOOL, &number_amount);
    ////        printf("Received %d numbers", number_amount);
    ////    }
    //
    //
    //    for (int i = 0; i < boardSize; ++i) {
    //        printf("Master - [%d, %d, %d]\n", board[i][0], board[i][1], board[i][2]);
    //
    //    }
    //    saveBoard(board, boardSize, iteration);
    //
    //
    //    cleanupBoard(board, boardSize);
    //    delete[] reqs, statuses;
    bool** board = new bool*[boardSize];
    for (int i = 0; i < boardSize; ++i) {
        BoardColumn bc;
        board[i] = new bool[boardSize];
        recv(bc, processesInCharge[i], SAVE_BOARD_PHASE_TAG, MPI_COMM_WORLD, boardSize);
        board[i] = bc.column;
    }

    saveBoard(board, boardSize, iteration);


    cleanupBoard(board, boardSize);
}
