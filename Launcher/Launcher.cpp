// Launcher.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>
#include <omp.h>
#include <Windows.h>
#include <mpi.h>
#include <thread>
#include "file_manager.h"
#include <iomanip>
#include <iostream>
#include "worker.h"
#include "boardGenerator.h"


/**
 * Returns a column that has all values set to false
 */
bool* getVirtualColumn(int boardSize) {
    bool* newColumn = new bool[boardSize];
    std::fill_n(newColumn, boardSize, false);

    return newColumn;
}

void cleanupBoard(bool** board, int boardSize) {
    for (int j = 0; j < boardSize; j++) {
        delete[] board[j];
    }
    delete[] board;
}

int main() {
    const int numberOfIterations = 300;
    const int boardSize = 2048;
    bool** board = randomBoard(boardSize);
    bool* virtualColumn = getVirtualColumn(boardSize);

    saveBoard(board, boardSize, -1);

    auto start = std::chrono::steady_clock::now();
    for (int i = 1; i <= numberOfIterations; i++) {
        bool** newBoard = serialIteration(board, boardSize, i, virtualColumn);
        cleanupBoard(board, boardSize);
        board = newBoard;
    }

    cleanupBoard(board, boardSize);
    delete[] virtualColumn;

    const auto end = std::chrono::steady_clock::now();
    const auto diff = end - start;


    printf("--ENDED %d iterations in total of %fs--\n", numberOfIterations,
           std::chrono::duration<double, std::milli>(diff / 1000).count());

    return 0;
}

//int main(int argc, char** argv) {
//    int numberOfProcesses;
//    int processRank;
//
//    // Inicjalizacja podsystemu MPI
//    MPI_Init(&argc, &argv);
//
//    // Pobierz rozmiar globalnego komunikatora
//    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
//    // Pobierz numer procesu w globalnym komunikatorze
//    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
//    printf("There are %d running procceses.\n", numberOfProcesses);
//    printf("I am process number %d.\n", processRank);
//    MPI_Finalize();
//    return 0;
//}


//int main() {
//    // Ustaw liczbę wątków na 15
//    omp_set_num_threads(15);
//    // Wypisze się jeden raz
//    printf("Hello World! Kod szeregowy\n");
//    // Poniższy blok wykona się w 15 wątkach
//#pragma omp parallel
//    {
//        Sleep(3000);
//        printf("Hello, jestem wątkiem nr %d\n", omp_get_thread_num());
//    }
//    return 0;
//
//}
