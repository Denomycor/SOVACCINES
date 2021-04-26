#ifndef SOTIME_H
#define SOTIME_H

#include <time.h>

void getTime(struct timespec* clock);

void getFormatedTime(struct tm* t, long* nsec);


#endif