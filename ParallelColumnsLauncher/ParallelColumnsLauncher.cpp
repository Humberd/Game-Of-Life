#include "stdafx.h"
#include "mpi.h"
#include <omp.h>
#include <stdlib.h>
#include <cstddef> // For offsetof macro

int master();

int main(int argc, char** argv) {
    int numberOfProcesses;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    if (rank == 0) {
        master();
        printf("There are %d running procceses.\n", numberOfProcesses);
        //        MPI_Send(helloStr, _countof(helloStr), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        //        MPI_Recv(helloStr, _countof(helloStr), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //        printf("Rank %d receiver string %s from Rank 0\n", rank, helloStr);
    }


    MPI_Finalize();
    return 0;
}

struct simple {
    int i;
    double d[3];
};


int master() {
    int membersize, maxsize;
    char* buffer;

    constexpr size_t num_members = 2;
    int lengths[num_members] = {1, 3};
    MPI_Aint offsets[num_members] = {
        offsetof(simple, i), offsetof(simple, d)
    };
    MPI_Datatype types[num_members] = {MPI_INT, MPI_DOUBLE};
    MPI_Datatype simple_type;
    MPI_Type_create_struct(num_members, lengths, offsets, types, &simple_type);
    MPI_Type_commit(&simple_type);


    return 0;
}

MPI_Datatype register_mpi_type(simple const& example) {
    constexpr std::size_t num_members = 3;
    int lengths[num_members] = {1, 1, 3};

    MPI_Aint offsets[num_members] = {
        offsetof(simple, x),
        offsetof(simple, y), offsetof(simple, vec)
    };
    MPI_Datatype types[num_members] = {
        MPI_INT, MPI_INT,
        MPI_DOUBLE
    };

    MPI_Datatype type;
    MPI_Type_struct(num_members, lengths, offsets, types, &type);
    MPI_Type_commit(&type);
    return type;

}


//int main(int argc, char* argv[]) {
//    int numberOfIterations = 1;
//    int boardSize = 64;
//    const int numberOfWorkers = 3;
//
//    omp_set_num_threads(numberOfWorkers);
//
//#pragma omp parallel default(none) firstprivate(numberOfIterations, boardSize)
//    {
//        const int threadId = omp_get_thread_num();
//        printf("Hello from thread %d\n", threadId);
//#pragma omp barrier
//
//        int counter = 0;
//#pragma omp for schedule (static)
//        for (int i = 0; i < 100; i++) {
//            printf("%d", omp_get_thread_num());
//            counter += 1;
//        }
//        printf("\n%d", counter);
//
//    }
//
//    return 0;
//}
