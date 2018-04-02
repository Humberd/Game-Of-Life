#pragma once
class Master {
private:
    int boardSize;
    int threads;
public:


    Master(int boardSize, int threads) : boardSize(boardSize),
                                         threads(threads) {
    }

    ~Master() = default;

    void sendColumnsInCharge();
};
