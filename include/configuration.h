#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdio.h>

FILE* openFile(char* name, char* mode);

int closeFile(FILE* file);

void readLine(FILE* file, char* mem);

void readNumber(FILE* file, int* mem);

#endif