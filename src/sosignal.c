/**
Grupo054
Afonso Esteves, fc54394
Diogo Fernandes, fc54458
Miguel Santos, fc54461
*/

#include "../include/main.h"
#include "../include/sotime.h"
#include "../include/sosignal.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



extern struct main_data* g_data;
extern struct communication_buffers* g_buffers;
extern struct semaphores* g_sems;

//function to be called when a signal is catched
void sighandler(int signum) {

    signal(SIGINT, sighandler);

    printf("Caught signal %d , terminating program", signum);
    stop_execution(g_data,g_buffers,g_sems);
    destroy_dynamic_memory(g_data);
    destroy_dynamic_memory(g_buffers->main_cli);
    destroy_dynamic_memory(g_buffers->cli_prx);
    destroy_dynamic_memory(g_buffers->prx_srv);
    destroy_dynamic_memory(g_buffers->srv_cli);
    destroy_dynamic_memory(g_buffers);
    destroy_dynamic_memory(g_sems->main_cli);
    destroy_dynamic_memory(g_sems->cli_prx);
    destroy_dynamic_memory(g_sems->prx_srv);
    destroy_dynamic_memory(g_sems->srv_cli);
    destroy_dynamic_memory(g_sems);
    exit(0); //guarentees the termination of the main process here
}



//alarm
void sinal_horario() {

    signal(SIGALRM, sinal_horario);
    int nr_processed = 0;

    for(int i = 0; i< g_data->n_servers; i++) {
        nr_processed += g_data->server_stats[i];
    }

    for(int i = 0; i < g_data->max_ops; i++){
        if(i<nr_processed) {
            semaphore_mutex_lock(g_sems->results_mutex);
            printf("op:%d status:%c start:%lu client:%d"
                    "client_time:%lu proxy:%d proxy_time:%lu"
                    "server:%d server_time:%lu end:%lu",
                    i, g_data->results[i].status,
                    g_data->results[i].start_time.tv_sec,
                    g_data->results[i].client,
                    g_data->results[i].client_time.tv_sec,
                    g_data->results[i].proxy,
                    g_data->results[i].proxy_time.tv_sec,
                    g_data->results[i].server,
                    g_data->results[i].server_time.tv_sec,
                    g_data->results[i].end_time.tv_sec
            );
            semaphore_mutex_unlock(g_sems->results_mutex);
        }
        else {
            printf("op:%d status:0", i);
        }
    }
}
