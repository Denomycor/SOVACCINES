#include "../include/sotime.h"
#include <time.h>


void getTime(struct timespec* clock){
    clock_gettime(CLOCK_REALTIME, clock);
}

void getFormatedTime(struct tm* t, long* nsec){
    struct timespec tf;
    getTime(&tf);
    *nsec = tf.tv_nsec;
    localtime_r(&tf.tv_sec, t);
}

void timespec_diff(const struct timespec *start, const struct timespec *stop, struct timespec *result){
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
}