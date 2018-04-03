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
    //    BoardColumn* newbcs = new BoardColumn[boardSize];
    //    std::fill_n(newbcs, boardSize, BoardColumn());
    //    MPI_Request* reqs = new MPI_Request[boardSize * 2];
    //    MPI_Status* statuses = new MPI_Status[boardSize * 2];
    //    for (int i = 0; i < boardSize; ++i) {
    //        MPI_Request* locReqs;
    //        locReqs = recvAsync(newbcs[i], processesInCharge[i], SAVE_BOARD_PHASE_TAG, MPI_COMM_WORLD, boardSize);
    //
    //        memcpy(reqs + i * 2, locReqs, 2);
    //        delete[] locReqs;
    //    }
    //
    //    MPI_Waitall(boardSize, reqs, statuses);
    //    printf("Master - Iteration %d - Receiving %d columns to save.\n",
    //           iteration, boardSize);
    //
    //
    //    bool** board = new bool*[boardSize];
    //    for (int i = 0; i < boardSize; ++i) {
    //        board[i] = newbcs[i].column;
    //        printf("columnIndex: %d\n", newbcs[i].columnIndex);
    //        printf("Master - [%d, %d, %d]\n", newbcs[i].column[0], newbcs[i].column[1], newbcs[i].column[2]);
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
        recv(bc, processesInCharge[i], SAVE_BOARD_PHASE_TAG, MPI_COMM_WORLD, boardSize);
        board[i] = bc.column;
        printf("Master - [%d, %d, %d]\n", bc.column[0], bc.column[1], bc.column[2]);
    }

    saveBoard(board, boardSize, iteration);


    cleanupBoard(board, boardSize);
}
