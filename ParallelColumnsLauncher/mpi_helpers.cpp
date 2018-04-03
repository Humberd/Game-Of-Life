#include "stdafx.h"
#include "mpi_helpers.h"
#include <cstddef>
#include <iterator>

MPI_Datatype register_mpi_type(BoardColumn const&) {
    constexpr std::size_t num_members = 5;
    int lengths[num_members] = {1, 0, 1, 1, 1};
    MPI_Aint offsets[num_members] = {
        offsetof(BoardColumn, iteration),
        offsetof(BoardColumn, column),
        offsetof(BoardColumn, columnIndex),
        offsetof(BoardColumn, leftColumnProcRank),
        offsetof(BoardColumn, rightColumnProcRank)
    };

    MPI_Datatype types[num_members] = {
        MPI_INT,
        MPI_C_BOOL,
        MPI_INT,
        MPI_INT,
        MPI_INT
    };
    MPI_Datatype type;
    MPI_Type_create_struct(num_members, lengths, offsets, types, &type);
    MPI_Type_commit(&type);
    return type;
}

void send(BoardColumn& e, int dest, int tag, MPI_Comm comm, int boardSize) {
    const MPI_Datatype type = register_mpi_type(e);
    MPI_Send(&e, 1, type, dest, tag, comm);
    MPI_Send(e.column, boardSize, MPI_C_BOOL, dest, tag + 1, comm);
    deregister_mpi_type(type);
}

MPI_Request* sendAsync(BoardColumn& e, int dest, int tag, MPI_Comm comm, int boardSize) {
    MPI_Request* reqs = new MPI_Request[2] {
        MPI_Request(),
        MPI_Request(),
    };
    const MPI_Datatype type = register_mpi_type(e);
    MPI_Isend(&e, 1, type, dest, tag, comm, &reqs[0]);
    MPI_Isend(e.column, boardSize, MPI_C_BOOL, dest, tag + 1, comm, &reqs[1]);
    deregister_mpi_type(type);

    return reqs;
}

MPI_Request* sendAsync(bool* column, int dest, int tag, MPI_Comm comm, int boardSize) {
    MPI_Request* req = new MPI_Request();
    MPI_Isend(column, boardSize, MPI_C_BOOL, dest, tag, comm, req);
    return req;
}

void recv(BoardColumn& e, int src, int tag, MPI_Comm comm, int boardSize) {
    const MPI_Datatype type = register_mpi_type(e);
    MPI_Recv(&e, 1, type, src, tag, comm, MPI_STATUS_IGNORE);
    e.column = new bool[boardSize];
    MPI_Recv(e.column, boardSize, MPI_C_BOOL, src, tag + 1, comm, MPI_STATUS_IGNORE);
    deregister_mpi_type(type);
}

MPI_Request* recvAsync(BoardColumn& e, int src, int tag, MPI_Comm comm, int boardSize) {
    MPI_Request* reqs = new MPI_Request[2]{
        MPI_Request(),
        MPI_Request(),
    };
    const MPI_Datatype type = register_mpi_type(e);
    MPI_Irecv(&e, 1, type, src, tag, comm, &reqs[0]);
    e.column = new bool[boardSize];
    MPI_Irecv(e.column, boardSize, MPI_C_BOOL, src, tag + 1, comm, &reqs[1]);
    deregister_mpi_type(type);
    return reqs;
}

MPI_Request* recvAsync(bool* column, int src, int tag, MPI_Comm comm, int boardSize) {
    MPI_Request* req = new MPI_Request();
    MPI_Irecv(column, boardSize, MPI_C_BOOL, src, tag + 1, comm, req);
    return req;
}

void deregister_mpi_type(MPI_Datatype type) {
    MPI_Type_free(&type);
}
