#ifndef STATS_H
#define STATS_H

#include <time.h>
#include <stdio.h>
#include "../include/main.h"

void writeStatisticsFile(const char* name,struct main_data* data);

void writeProcessStats(FILE *stream,struct main_data* data);

void writeOpStatistics(FILE* stream,struct operation* op, struct main_data* data);

void writeNumberOp(FILE *stream,struct operation* op,int i);

void writeStatus(FILE *stream,struct operation* op,int i);

void writeIds(FILE *stream,struct operation* op,int i);

void writeLocalsTime(FILE* stream,struct operation* op,int i);


#endif