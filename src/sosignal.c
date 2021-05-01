/**
Grupo054
Afonso Esteves, fc54394
Diogo Fernandes, fc54458
Miguel Santos, fc54461
*/

#include "../include/main.h"
#include "../include/sotime.h"
#include <signal.h>



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

    for(int i = 0; i < g_data->max_ops; i++){
        if(i<server_stats) {
            printf("op:%d status:%s start:%d client:%d"
                    "client_time:%d proxy:%d proxy_time:%d"
                    "server:%d server_time:%d end:%d",
                    i, g_data->results[i].status,
                    g_data->results[i].start_time,
                    g_data->results[i].client,
                    g_data->results[i].client_time,
                    g_data->results[i].proxy,
                    g_data->results[i].proxy_time,
                    g_data->results[i].server,
                    g_data->results[i].server_time,
                    g_data->results[i].end_time
            );
        }
        else {
            printf("op:%d status:0", i);
        }
    }
}
