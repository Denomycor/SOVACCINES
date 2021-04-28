#include "../include/log.h"
#include "../include/sotime.h"
#include "../include/configuration.h"
#include <stdio.h>

void logT(const char* filename, const char* interacao, const int* arg){
    FILE* file = openFile(filename, "w");
    
    logTime(file);
    fputs(interacao, file);

    if(arg!=NULL){
        char conv[10];
        sprintf(conv, "%d", *arg);
        fputs(conv, file);
    }

    fputs("\n", file);
    closeFile(file);
}

void logTime(FILE* f){
    struct tm tf;
    long nsec;
    getFormatedTime(&tf, &nsec);
    char temp[100];
    strftime(temp, 100, "%Y-%m-%d %H:%M:%S.", &tf);
    fputs(temp, f);
    sprintf(temp, "%ld", nsec);
    fputs(temp, f);
    fputs(" ", f);
}