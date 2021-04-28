#include "../include/configuration.h"
#include <stdio.h>


FILE* openFile(const char* name, const char* mode){
    FILE* res = fopen(name, mode);
    if(res==NULL){
        perror("Couldnt find/open the file");
        exit(-1);
    }else{
        if(ferror(res)!=0){
            perror("Unexpected error in file");
            exit(-1);
        }else{
            return res;
        }
    }
}

int closeFile(FILE* file){
    return fclose(file);
}

void readLine(FILE* file, char* mem){
    fscanf(file, "%s", mem);
}

void readNumber(FILE* file, int* mem){
    fscanf(file, "%d", mem);
}



//int fputs(char* line, FILE* file) escreve line para file
//int fputs(char* line, int max, File* file) le no maximo max- caracteres de file para line
//int fscanf(...)
//int fprintf(...)