#ifndef LOG_H
#define LOG_H

void logT(const char* filename, const char* interacao, const int* arg);

void logTime(FILE* f);

void logTimeFormated(FILE* f, char* buffer, struct tm* time, long nsec);

#endif

