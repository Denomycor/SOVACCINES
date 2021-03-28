#include "main.h"

void main_args(int argc, char* argv[], struct main_data* data) {
    data->max_ops = argv[1];
    data->buffers_size = argv[2];
    data->n_clients = argv[3];
    data->n_proxies = argv[4];
    data->n_servers = argv[5];
}

void create_dynamic_memory_buffers(struct main_data* data) {
    int size = data->n_clients + data->client_stats + data->n_proxies + 
                data->proxy_stats + data->n_servers + data->server_stats;
    create_dynamic_memory(size);
}

void create_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers) {
    int size = 1 + /*int *terminated*/
                sizeof(data->results) + sizeof(buffers->main_cli) + sizeof(buffers->cli_prx) +
                sizeof(buffers->srv_cli) + sizeof(buffers->prx_srv);
    
    char name[] = "shared_memory";
    create_shared_memory(name,size);

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

void user_interaction(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    char interacao[];
    scanf(%s,interacao);

    switch(interacao) {
        case 'op':
            create_request(sizeof(data->results),buffers,data,sems);
        break;

        case 'read':
            read_answer(data,sems);
        break;

        case 'stop':
            stop_execution(data,buffers,sems);
        break;

        case 'help':
            printf("The op option creates a new operation/n");
            printf("The read option checks the status of the operation/n");
            printf("The stop option ends the program/n");
        break;

        default:
        perror("There is no interaction with that name/n");
    }
}

void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){ 
 if(*op_counter < *(data->max_ops)){
     op new_op;
     new_op.id =  *op_counter;
    produce_begin(sems->main_cli);

    read_rnd_acess_buffer(buffers->main_cli,*(data->buffers_size),new_op);

    produce_end(sems_main_cli);
    printf("Operation ID: %d \n", *(new_op->id));
    (*op_counter)++;
 }
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


void write_statistics(struct main_data* data){
 printf("Operações processadas por cada cliente: %d \n",*(data->client_stats));
 printf("Operações processadas por cada proxy: %d \n",(data->proxy_stats));
 printf("Operações processadas por cada servidor: %d \n",(data->server_stats));
}

void destroy_dynamic_memory_buffers(struct main_data* data){
 destroy_dynamic_memory(data->client_pids);
 destroy_dynamic_memory(data->proxy_pids);
 destroy_dynamic_memory(data->server_pids);
 destroy_dynamic_memory(data->client_stats);
 destroy_dynamic_memory(data->proxy_stats);
 destroy_dynamic_memory(data->server_stats);

}

void destroy_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers){
 destroy_shared_memory("SHM_MAIN_CLI_BUFFER",buffers->main_cli,data->buffers_size);
 destroy_shared_memory("SHM_CLI_PRX_BUFFER",buffers->cli_prx,data->buffers_size);
 destroy_shared_memory("SHM_PRX_SRV_BUFFER",buffers->prx_srv,data->buffers_size);
 destroy_shared_memory("SHM_SRV_CLI_BUFFER",buffers->srv_cli,data->buffers_size);

}

void destroy_semaphores(struct semaphores* sems){
 semaphore_destroy(sems->main_cli->full);
 semaphore_destroy(sems->main_cli->empty);
 semaphore_destroy(sems->main_cli->mutex);
 semaphore_destroy(sems->cli_prx->full);
 semaphore_destroy(sems->cli_prx->empty);
 semaphore_destroy(sems->cli_prx->mutex);
 semaphore_destroy(sems->prx_srv->full);
 semaphore_destroy(sems->prx_srv->empty);
 semaphore_destroy(sems->prx_srv->mutex);
 semaphore_destroy(sems->srv_cli->full);
 semaphore_destroy(sems->srv_cli->empty);
 semaphore_destroy(sems->srv_cli->mutex);
}
