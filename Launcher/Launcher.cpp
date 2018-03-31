// Launcher.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>
#include <omp.h>
#include <Windows.h>
#include <mpi.h>
#include <string>
#include <thread>
#include "file_manager.h"
#include <iomanip>
#include <iostream>


bool** randomizeBoard(int boardSize) {
    bool** board = new bool*[boardSize];
    for (int i = 0; i < boardSize; i++) {
        bool defaultValue = i % 2;
        bool* newColumn = new bool[boardSize];
        std::fill_n(newColumn, boardSize, defaultValue);
        board[i] = newColumn;
    }


    return board;
}

/**
 * Returns a column that has all values set to false
 */
bool* getVirtualColumn(int boardSize) {
    bool* newColumn = new bool[boardSize];
    std::fill_n(newColumn, boardSize, false);

    return newColumn;
}

int main() {
    const int boardSize = 10;
    bool** board = randomizeBoard(boardSize);
    bool* virtualColumn = getVirtualColumn(boardSize);

    saveBoard(board, boardSize, -1);

    for (int i = 0; i < boardSize; i++) {
        if (i == 0) {

        } else if (i == boardSize - 1) {

        } else {

        }
    }

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
