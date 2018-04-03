#pragma once
#include <mpi.h>

struct BoardColumn {
    int iteration;
    bool* column;
    int columnIndex;
    int leftColumnProcRank;
    int rightColumnProcRank;
};

MPI_Datatype register_mpi_type(BoardColumn const&);

void send(BoardColumn& e, int dest, int tag, MPI_Comm comm, int boardSize);

MPI_Request* sendAsync(BoardColumn& e, int dest, int tag, MPI_Comm comm, int boardSize);

MPI_Request* sendAsync(bool* column, int dest, int tag, MPI_Comm comm, int boardSize);

void recv(BoardColumn& e, int src, int tag, MPI_Comm comm, int boardSize);

MPI_Request* recvAsync(BoardColumn& e, int src, int tag, MPI_Comm comm, int boardSize);

MPI_Request* recvAsync(bool* column, int src, int tag, MPI_Comm comm, int boardSize);

void deregister_mpi_type(MPI_Datatype type);
