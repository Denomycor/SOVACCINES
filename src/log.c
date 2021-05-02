#include "../include/log.h"
#include "../include/sotime.h"
#include "../include/configuration.h"
#include <stdio.h>

void logTime(FILE* f){
    struct tm tf;
    long nsec;
    getFormatedTime(&tf, &nsec);
    char temp[100];
    logTimeFormated(f, temp, &tf, nsec);
    fputs(" ", f);
}

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

void logTimeFormated(FILE* f, char* buffer, struct tm* time, long nsec){
    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S.", time);
    fputs(buffer, f);
    sprintf(buffer, "%ld", nsec);
    fputs(buffer, f);
    
}

