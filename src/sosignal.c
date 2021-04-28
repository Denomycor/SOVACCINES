/**
Grupo054
Afonso Esteves, fc54394
Diogo Fernandes, fc54458
Miguel Santos, fc54461
*/

#include "../include/main.h"
#include <signal.h>

void sighandler(int signum,struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems) {
    printf("Caught signal %d , terminating program", signum);
    stop_execution(struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems); //this is supposed to be a call not a declaration
}

void sinal_horario(int alarm_time) {
    printf("write statistics from statistics_filename");
    alarm(alarm_time);                                      //keeps the alarm running with already given value
}
