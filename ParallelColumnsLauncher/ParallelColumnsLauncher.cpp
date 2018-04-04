#include "stdafx.h"
#include "mpi.h"
#include <omp.h>
#include <vector>
#include "defs.h"
#include "Slave.h"
#include "Master.h"
#include <windows.h>
#include "globals.h"

int main(int argc, char** argv) {
    int threads;
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &threads);

    int numberOfIterations = 0;
    int boardSize = 0;
    char* boardInitPattern;
    if (argc < 4) {
        if (rank == MASTER_RANK) {
            printf("Program requires at least 3 arguments: boardSize, numberOfIterations, boardInitPattern ");
            printf("and optional 4th argument indicating if debug logging should be enabled.");
        }
        MPI_Finalize();
        return 1;
    }

    boardSize = atoi(argv[1]);
    numberOfIterations = atoi(argv[2]);
    boardInitPattern = argv[3];

    if (argc == 5) {
        std::string argDebugEnabled(argv[4]);
        if (argDebugEnabled == "true") {
            debugEnabled = true;
        } else {
            debugEnabled = false;
        }
    }

    if (rank == MASTER_RANK) {
        printf("Initiating program with:\n");
        printf("boardSize=%d\n", boardSize);
        printf("numberOfIterations=%d\n", numberOfIterations);
        printf("boardInitPattern=%s\n", boardInitPattern);
        printf("debugEnabled=%hhd\n", debugEnabled);
    }

    if (rank == MASTER_RANK) {
        printf("There are %d running procceses.\n", threads);
        Master master(boardSize, threads);
        master.sendColumnsInCharge();
        for (int i = 1; i <= numberOfIterations; ++i) {
            master.recvColumnsToSave(i);
        }
    } else {
        Slave slave(rank, boardSize);
        slave.recvColumnsInCharge();
        if (slave.bcs.empty()) {
            printf_debug("Slave %d received no columns to be in charge of. Terminating peacefully...\n", rank);
            MPI_Finalize();
            return 0;
        }
        for (int i = 1; i <= numberOfIterations; ++i) {
            slave.iteration(i);
            //            printf("Slave %d - Iteration %d - ")
            slave.saveColumns(i);
        }
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
