#pragma once
#include <string>

class Master {
private:
    int boardSize;
    int threads;
public:
    /* index is a column, value is a process in charge of this column */
    int* processesInCharge;


    Master(int boardSize, int threads) : boardSize(boardSize),
                                         threads(threads) {
        processesInCharge = new int[boardSize];
    }

    ~Master() {
        delete[] processesInCharge;
    }

    void sendColumnsInCharge(std::string boardInitPattern);

    void recvColumnsToSave(int iteration);
};
