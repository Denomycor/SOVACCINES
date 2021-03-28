#include "../include/main.h"

void main_args(int argc, char* argv[], struct main_data* data) {
    data->max_ops = argv[1];
    data->buffers_size = argv[2];
    data->n_clients = argv[3];
    data->n_proxies = argv[4];
    data->n_servers = argv[5];
}

void create_dynamic_memory_buffers(struct main_data* data) {
    data->client_pids = create_dynamic_memory(data->n_clients*sizeof(int));
    data->proxy_pids = create_dynamic_memory(data->n_proxies*sizeof(int));
    data->server_pids = create_dynamic_memory(data->n_servers*sizeof(int));
    data->client_stats = create_dynamic_memory(sizeof(int));
    data->proxy_stats = create_dynamic_memory(sizeof(int));
    data->server_stats = create_dynamic_memory(sizeof(int));

}

void create_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers) {
    data->results = create_shared_memory(STR_SHM_RESULTS, data->max_ops*sizeof(struct operation));
    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
    buffers->main_cli = create_shared_memory(STR_SHM_MAIN_CLI_PTR, sizeof(struct rnd_access_buffer));
    buffers->cli_prx = create_shared_memory(STR_SHM_CLI_PRX_PTR, sizeof(struct circular_buffer));
    buffers->prx_srv = create_shared_memory(STR_SHM_PRX_SRV_PTR, sizeof(struct rnd_access_buffer));
    buffers->srv_cli = create_shared_memory(STR_SHM_SRV_CLI_PTR, sizeof(struct circular_buffer));
    
    buffers->main_cli->elems = create_shared_memory(STR_SHM_MAIN_CLI_BUFFER, data->buffers_size*sizeof(struct operation)+data->buffers_size*sizeof(int));
    buffers->main_cli->flags = buffers->main_cli->elems + data->buffers_size*sizeof(struct operation);

    buffers->cli_prx->elems = create_shared_memory(STR_SHM_CLI_PRX_BUFFER, data->buffers_size*sizeof(struct operation));
    
    buffers->prx_srv->elems = create_shared_memory(STR_SHM_PRX_SRV_BUFFER, data->buffers_size*sizeof(struct operation)+data->buffers_size*sizeof(int));
    buffers->prx_srv->flags = buffers->prx_srv->elems + data->buffers_size*sizeof(struct operation);

    buffers->srv_cli->elems = create_shared_memory(STR_SHM_SRV_CLI_BUFFER, data->buffers_size*sizeof(struct operation));
}

void create_semaphores(struct main_data* data, struct semaphores* sems){
 sems->main_cli->ful = semaphore_create("sem_main_cli_full",0);
 sems->main_cli->empty = semaphore_create("sem_main_cli_empty",*(data->buffers_size));
 sems->main_cli->mutex = semaphore_create("sem_main_cli_mutex",1);
 sems->cli_prx->full = semaphore_create("sem_cli_prx_full",0);
 sems->cli_prx->empty = semaphore_create("sem_cli_prx_empty",*(data->buffers_size));
 sems->cli_prx->mutex = semaphore_create("sem_cli_prx_mutex",1);
 sems->prx_srv->full = semaphore_create("sem_prx_srv_full",0);
 sems->prx_srv->empty = semaphore_create("sem_prx_srv_empty",*(data->buffers_size));
 sems->prx_srv->mutex = semaphore_create("sem_prx_srv_mutex",1);
 sems->srv_cli->full = semaphore_create("sem_srv_cli_full",0);
 sems->srv_cli->empty = semaphore_create("sem_srv_cli_empty",*(data->buffers_size));
 sems->srv_cli->mutex = semaphore_create("sem_srv_cli_mutex",1);
 sems->results_mutex = semaphore_create("sem_results_mutex",1)
}

void launch_processes(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems) {

    /*Clientes*/
    for(int i = 0; i < data.n_clients;i++) {
        clients_pids[i] = launch_process(*(data.client_pids + i),0,buffers,data,sems);
    }

    /*Proxy*/
    for(int i = 0; i < data.n_proxies;i++) {
        proxy_pids[i] = launch_process(*(data.proxy_pids + i),1,buffers,data,sems);
    }

    /*Server*/
    for(int i = 0; i < data.n_servers;i++) {
        server_pids[i] = launch_process(*(data.server_pids + i),2,buffers,data,sems);
    }
   
}

void user_interaction(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    char* interacao;
    scanf("%s",interacao);

    switch(*interacao) {
        case 'op':
            create_request(error,buffers,data,sems);
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
     struct operation new_op;
     new_op.id =  *op_counter;
     produce_begin(sems->main_cli);

     write_rnd_acess_buffer(buffers->main_cli,*(data->buffers_size),new_op);

     produce_end(sems_main_cli);
     printf("Operation ID: %d \n", *(new_op->id));
     (*op_counter)++;
   }
}

void read_answer(struct main_data* data, struct semaphores* sems){
    int id;
    scanf("%d", &id);
    semaphore_mutex_lock(sems->results_mutex);
    for(int i=0; i<data->max_ops; i++){
        struct operation* holder = data->results+i;
        if(holder->id == if){
            printf("Status da operacao: %d\n", holder->status);
            printf("Processo Cliente: %d\n", holder->client);
            printf("Processo Proxy: %d\n", holder->proxy);
            printf("Processo Servidor: %d\n", holder->server);
            
            semaphore_mutex_unlock(sems->results_mutex);
            return
        }
    }

    printf("Nao foi encontrada operacao com id: %d\n", id);

    semaphore_mutex_unlock(sems->results_mutex);
}


void stop_execution(struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems) {
    *(data->terminate) = 1;
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
        produce_end(sems.cli_prx[i]);
    }

    /*Proxy*/
    for(int i = 0; i < data.n_proxies;i++) {
        produce_end(sems.prx_srv[i]);
    }

    /*Server*/
    for(int i = 0; i < data.n_servers;i++) {
        produce_end(sems.srv_cli[i]);
    }
}

void wait_processes(struct main_data* data) {
    /*Clientes*/
    for(int i = 0; i < data.n_clients;i++) {
        wait_process(data.client_pids[i]);
    }

    /*Proxy*/
    for(int i = 0; i < data.n_proxies;i++) {
        wait_process(data.proxy_pids[i]));
    }

    /*Server*/
    for(int i = 0; i < data.n_servers;i++) {
        wait_process(data.server_pids[i]);
    }
}


void write_statistics(struct main_data* data){
 printf("Operações processadas por cada cliente: %d \n",*(data->client_stats));
 printf("Operações processadas por cada proxy: %d \n",*(data->proxy_stats));
 printf("Operações processadas por cada servidor: %d \n",*(data->server_stats));
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
 destroy_shared_memory(STR_SHM_MAIN_CLI_BUFFER,buffers->main_cli->elems,data->buffers_size*sizeof(struct operation));
 destroy_shared_memory(STR_SHM_CLI_PRX_BUFFER,buffers->cli_prx->elems,data->buffers_size*sizeof(struct operation));
 destroy_shared_memory(STR_SHM_PRX_SRV_BUFFER,buffers->prx_srv->elems,data->buffers_size*sizeof(struct operation));
 destroy_shared_memory(STR_SHM_SRV_CLI_BUFFER,buffers->srv_cli->elems,data->buffers_size*sizeof(struct operation));
 
 
 destroy_shared_memory(STR_SHM_MAIN_CLI_PTR, buffers->main_cli, sizeof(struct rnd_access_buffer));
 destroy_shared_memory(STR_SHM_CLI_PRX_PTR, buffers->cli_prx, sizeof(struct circular_buffer));
 destroy_shared_memory(STR_SHM_PRX_SRV_PTR, buffers->prx_srv, sizeof(struct rnd_access_buffer));
 destroy_shared_memory(STR_SHM_SRV_CLI_PTR, buffers->srv_cli, sizeof(struct circular_buffer));
 destroy_shared_memory(STR_SHM_RESULTS, data->results, data->max_ops*sizeof(struct operation));
 destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));
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
 semaphore_destroy(sems->results_mutex);
}
