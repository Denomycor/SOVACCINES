#ifndef SOTIME_H
#define SOTIME_H

#include <time.h>

void getTime(struct timespec* clock);

void getFormatedTime(struct tm* t, long* nsec);

void timespec_diff(const struct timespec *start, const struct timespec *stop, struct timespec *result);

#endif