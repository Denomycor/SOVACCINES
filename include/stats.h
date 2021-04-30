#include <time.h>
#include <stdio.h>
#include "main.h"

void writeStatisticsFile(FILE *stream,struct main_data* data,int i);

void writeProcessStats(FILE *stream,struct main_data* data,int i);

void writeNumberOp(FILE *stream,struct operation* op,int i);

void writeStatus(FILE *stream,struct operation* op,int i);

void writeIds(FILE *stream,struct operation* op, int i);

void writeLocalsTime(FILE *stream,struct operation* op,int i);

/*void writeTotalTime(FILE *stream);*/
