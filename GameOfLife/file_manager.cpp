#include "stdafx.h"
#include "file_manager.h"
#include <ctime>
#include <string>
#include <windows.h>
#include <cstdio>
#pragma warning(disable:4996)

void WritePGM(char* fname, bool** board, int boardSize) {
    FILE* file;

    fopen_s(&file, fname, "wt");
    if (fname == NULL) {
        return;
    }
    fprintf(file, "P5 %d %d 1 ", boardSize, boardSize);
    for (int i = 0; i < boardSize; i++) {
        fwrite(board[i], sizeof(bool), boardSize, file);
    }
    fclose(file);
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%H.%M.%S", &tstruct);

    return buf;
}

void saveBoard(bool** board, int boardSize, int iteration) {
    std::string fileName = "screens";
    createFolder(fileName);
    fileName += "/" + currentDateTime();
    createFolder(fileName);

    if (iteration <= 0) {
        fileName += "/begin.pgm";
    } else {
        fileName += "/iteration_" + std::to_string(iteration) + ".pgm";
    }
    WritePGM(_strdup(fileName.c_str()), board, boardSize);
}

void createFolder(std::string path) {
    std::wstring stemp = std::wstring(path.begin(), path.end());
    if (CreateDirectory(stemp.c_str(), NULL)) {
        // Directory created
    } else if (ERROR_ALREADY_EXISTS == GetLastError()) {
        // Directory already exists
    } else {
        // Failed for some other reason
    }
}
