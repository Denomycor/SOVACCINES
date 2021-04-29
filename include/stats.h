#include <time.h>
#include <stdio.h>
#include "main.h"

const char* getFileName (FILE *stream);

void writeProcessStats(FILE *stream,struct main_data* data);

void writeNumberOP(FILE *stream,struct main_data* data);

void writeStatus(FILE *stream,int OP);

void writeClientId(FILE *stream,struct main_data* data);

void writeProxyId(FILE *stream,struct main_data* data);

void writeServerId(FILE *stream,struct main_data* data);

void writeLocalsTime(FILE *stream);

/*void writeTotalTime(FILE *stream);*/