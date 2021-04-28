/**
Grupo054
Afonso Esteves, fc54394
Diogo Fernandes, fc54458
Miguel Santos, fc54461
*/

#include <signal.h>

void sighandler(int signum,struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems);

void sinal_horario(int alarm_time);

