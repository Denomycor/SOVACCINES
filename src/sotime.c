#include "../include/sotime.h"
#include <time.h>


void getTime(struct timespec* clock){
    clock_gettime(CLOCK_REALTIME, clock);
}

void getFormatedTime(struct tm* t, long* nsec){
    struct timespec tf;
    getTime(tf);
    *nsec = tf.tv_nsec;
    localtime_r(&tf.tv_sec, t);
}