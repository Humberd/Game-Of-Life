#include "stdafx.h"
#include "mpi.h"
#include <omp.h>
#include <stdlib.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int numberOfProcesses;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    if (rank == 0) {
        printf("There are %d running procceses.\n", numberOfProcesses);
        char helloStr[] = "Hello World";
        MPI_Send(helloStr, _countof(helloStr), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        char helloStr[12];
        MPI_Recv(helloStr, _countof(helloStr), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d receiver string %s from Rank 0\n", rank, helloStr);
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
