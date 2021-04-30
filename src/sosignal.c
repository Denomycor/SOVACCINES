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
    stop_execution(data,buffers,sems);
    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_cli);
    destroy_dynamic_memory(buffers->cli_prx);
    destroy_dynamic_memory(buffers->prx_srv);
    destroy_dynamic_memory(buffers->srv_cli);
    destroy_dynamic_memory(buffers);
    destroy_dynamic_memory(sems->main_cli);
    destroy_dynamic_memory(sems->cli_prx);
    destroy_dynamic_memory(sems->prx_srv);
    destroy_dynamic_memory(sems->srv_cli);
    destroy_dynamic_memory(sems);
}

void sinal_horario(int alarm_time,struct main_data* data) {
    for(int i = 0; i < *data->max_ops; i++){
        if(i<server_stats) {
            printf("op:%d status:%s start:%d client:%d
                    client_time:%d proxy:%d proxy_time:%d
                    server:%d server_time:%d end:%d",
                    i, *data->results[i]->status,
                    *data->results[i]->start_time,
                    *data->results[i]->client,
                    *data->results[i]->client_time,
                    *data->results[i]->proxy,
                    *data->results[i]->proxy_time,
                    *data->results[i]->server,
                    *data->results[i]->server_time,
                    *data->results[i]->end_time
            );
        }
        else {
            printf("op:%d status:0", i);
        }
    }
    alarm(alarm_time);                        //keeps the alarm running with already given value
}
