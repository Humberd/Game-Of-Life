#pragma once
#include <mpi.h>

struct BoardColumn {
    int iteration;
    bool* column;
    int columnIndex;
};

MPI_Datatype register_mpi_type(BoardColumn const&);

void send(BoardColumn& e, int dest, int tag, MPI_Comm comm, int boardSize);

void recv(BoardColumn& e, int src, int tag, MPI_Comm comm, int boardSize);

void deregister_mpi_type(MPI_Datatype type);
