#include "stdafx.h"
#include "file_manager.h"

void WritePGM(char* fname, int boardSize, bool** board) {
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
