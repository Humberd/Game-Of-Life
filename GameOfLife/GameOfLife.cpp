// GameOfLife.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mpi.h>
int main(int argc, char *argv[])
{
    int npes;
    int myrank;

    // Inicjalizacja podsystemu MPI
    MPI_Init(&argc, &argv);

    // Pobierz rozmiar globalnego komunikatora
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    // Pobierz numer procesu w globalnym komunikatorze
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    printf("Jestem %d procesem z %d\n", myrank, npes);
    MPI_Finalize();
    return 0;
}