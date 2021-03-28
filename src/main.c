#include "main.h"

void main_args(int argc, char* argv[], struct main_data* data) {
    data->max_ops = argv[1];
    data->buffers_size = argv[2];
    data->n_clients = argv[3];
    data->n_proxies = argv[4];
    data->n_servers = argv[5];
}

void stop_execution(struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems) {
    data->terminate* = 1;
    wakeup_processes(data,sems);
    wait_processes(data);
    write_statistics(data);
    destroy_semaphores(sems);
    destroy_shared_memory_buffers(data,buffers);
    destroy_dynamic_memory_buffers(data);
}

void create_dynamic_memory_buffers(struct main_data* data) {
    int size = data->n_clients + data->client_stats + data->n_proxies + 
                data->proxy_stats + data->n_servers + data->server_stats;
    create_dynamic_memory(size);
}

void launch_processes(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems) {

    /*Clientes*/
    for(int i = 0; i < data.n_clients;i++) {
        launch_process(*(data.client_pids + i),0,buffers,data,sems);
    }

    /*Proxy*/
    for(int i = 0; i < data.n_proxies;i++) {
        launch_process(*(data.proxy_pids + i),1,buffers,data,sems);
    }

    /*Server*/
    for(int i = 0; i < data.n_servers;i++) {
        launch_process(*(data.server_pids + i),2,buffers,data,sems);
    }
   
}

void wait_processes(struct main_data* data) {
    /*Clientes*/
    for(int i = 0; i < data.n_clients;i++) {
        wait_process(*(data.client_pids + i));
    }

    /*Proxy*/
    for(int i = 0; i < data.n_proxies;i++) {
        wait_process(*(data.proxy_pids + i));
    }

    /*Server*/
    for(int i = 0; i < data.n_servers;i++) {
        wait_process(*(data.server_pids + i));
    }
}

void wakeup_processes(struct main_data* data, struct semaphores* sems) {
    /*Main*/
    produce_end(sems->main_cli);
    
    /*Clientes*/
    for(int i = 0; i < data.n_clients;i++) {
        produce_end(*(sems.cli_prx + i));
    }

    /*Proxy*/
    for(int i = 0; i < data.n_proxies;i++) {
        produce_end(*(sems.prx_srv + i));
    }

    /*Server*/
    for(int i = 0; i < data.n_servers;i++) {
        produce_end(*(sems.srv_cli + i));
    }
}

void write_statistics(struct main_data* data){
 printf("Operações processadas por cada cliente: %d \n",*(data->client_stats));
 printf("Operações processadas por cada proxy: %d \n",(data->proxy_stats));
 printf("Operações processadas por cada servidor: %d \n",(data->server_stats));
}
