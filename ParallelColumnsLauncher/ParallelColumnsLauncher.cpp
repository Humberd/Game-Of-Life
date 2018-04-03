#include "stdafx.h"
#include "mpi.h"
#include <omp.h>
#include <vector>
#include "defs.h"
#include "Slave.h"
#include "Master.h"
#include <windows.h>

int main(int argc, char** argv) {
    int threads;
    int rank;
    int boardSize = 3;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &threads);

    MPI_Comm cart;
    int dims[1] = {threads - 1};
    int periods[1] = {0};
    MPI_Cart_create(MPI_COMM_WORLD, std::size(dims), dims, periods, 1, &cart);

    if (rank == MASTER_RANK) {
        printf("There are %d running procceses.\n", threads);
        Master master(boardSize, threads);
        master.sendColumnsInCharge();

    } else {
        Slave slave(rank, boardSize);
        slave.recvColumnsInCharge();
        if (slave.bcs.empty()) {
            printf("Slave %d received no columns to be in charge of. Terminating peacefully...", rank);
            MPI_Finalize();
            return 0;
        }
        slave.iteration();
    }

    MPI_Finalize();
    return 0;
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
