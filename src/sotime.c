#include "../include/sotime.h"
#include <time.h>


void getTime(struct timespec* clock){
    clock_gettime(CLOCK_REALTIME, clock);
}