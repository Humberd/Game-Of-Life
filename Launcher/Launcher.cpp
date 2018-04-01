// Launcher.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "serial_launcher.h"


int main(int argc, char* argv[]) {
    launch_serial(argc, argv);

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
