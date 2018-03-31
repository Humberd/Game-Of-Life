#pragma once
#include <string>


void WritePGM(char* fname, bool** board, int boardSize);

std::string currentDateTime();

void saveBoard(bool** board, int boardSize, int iteration);

void createFolder(std::string path);