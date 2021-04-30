#include <time.h>
#include <stdio.h>
#include "main.h"

void writeStatisticsFile(FILE *stream,struct main_data* data,int i);

void writeProcessStats(FILE *stream,struct main_data* data,int i);

void writeNumberOP(FILE *stream,struct main_data* data);

void writeStatus(FILE *stream,struct operation* op,int i);

void writeIds(FILE *stream,struct main_data* data);

void writeLocalsTime(FILE *stream);

/*void writeTotalTime(FILE *stream);*/
