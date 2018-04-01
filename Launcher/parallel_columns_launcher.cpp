#include "stdafx.h"
#include <omp.h>
#include "parallel_columns_launcher.h"

int launch_parallel_columns(int argc, char* argv[]) {
    int numberOfIterations = 1;
    int boardSize = 64;
    const int numberOfWorkers = 3;

    omp_set_num_threads(numberOfWorkers);

#pragma omp parallel default(none) firstprivate(numberOfIterations, boardSize)
    {
        const int threadId = omp_get_thread_num();
        printf("Hello from thread %d\n", threadId);
#pragma omp barrier

        int counter = 0;
#pragma omp for schedule (static)
        for (int i = 0; i < 100; i++) {
            printf("%d", omp_get_thread_num());
            counter += 1;
        }
        printf("\n%d", counter);

    }

    return 0;
}
