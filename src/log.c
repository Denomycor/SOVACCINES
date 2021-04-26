#include "../include/log.h"
#include "../include/sotime.h"
#include "../include/configuration.h"
#include <stdio.h>

void log(const char* filename, const char* interacao){
    FILE* file = openFile(filename, "w");
    logTime(file);

    fputs(interacao, file);

    closeFile(file);
}

void logTime(FILE* f){
    struct tm tf;
    long nsec;
    getFormatedTime(&tf, &nsec);
    char temp[100];
    strftime(temp, 100, "%Y-%m-%d %H:%M:%S .", &tf);
    fputs(temp, f);
    sprintf(temp, "%d", nsec);
    fputs(temp, f);
    fputs(" ", f);
}