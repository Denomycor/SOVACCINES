/**
Grupo054
Afonso Esteves, fc54394
Diogo Fernandes, fc54458
Miguel Santos, fc54461
*/

#include "../include/main.h"
#include "../include/process.h"
#include "../include/configuration.h"
#include "../include/sotime.h"
#include "../include/log.h"
#include "../include/sosignal.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>


struct main_data* g_data;
struct communication_buffers* g_buffers;
struct semaphores* g_sems;

void main_args(int argc, char* argv[], struct main_data* data) {

    FILE* input = openFile(argv[1], "r");

    readNumber(input, &data->max_ops);
    readNumber(input, &data->buffers_size);
    readNumber(input, &data->n_clients);
    readNumber(input, &data->n_proxies);
    readNumber(input, &data->n_servers);
    
    char reader[100]; //used as a temporary writing buffer for file reading ops
    
    readLine(input, reader);
    data->log_filename = create_dynamic_memory(strlen(reader)+1); //+1 for the null termination character
    strncpy(data->log_filename, reader, strlen(reader)+1);

    readLine(input, reader);
    data->statistics_filename = create_dynamic_memory(strlen(reader)+1); //+1 for the null termination character
    strncpy(data->statistics_filename, reader, strlen(reader)+1);

    readNumber(input, &data->alarm_time);

    closeFile(input);
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
    buffers->main_cli->flags = (void*)(buffers->main_cli->elems + data->buffers_size);

    buffers->cli_prx->elems = create_shared_memory(STR_SHM_CLI_PRX_BUFFER, data->buffers_size*sizeof(struct operation));
    
    buffers->prx_srv->elems = create_shared_memory(STR_SHM_PRX_SRV_BUFFER, data->buffers_size*sizeof(struct operation)+data->buffers_size*sizeof(int));
    buffers->prx_srv->flags = (void*)(buffers->prx_srv->elems + data->buffers_size);

    buffers->srv_cli->elems = create_shared_memory(STR_SHM_SRV_CLI_BUFFER, data->buffers_size*sizeof(struct operation));
}

void create_semaphores(struct main_data* data, struct semaphores* sems){
 sems->main_cli->full = semaphore_create("sem_main_cli_full",0);
 sems->main_cli->empty = semaphore_create("sem_main_cli_empty",data->buffers_size);
 sems->main_cli->mutex = semaphore_create("sem_main_cli_mutex",1);
 sems->cli_prx->full = semaphore_create("sem_cli_prx_full",0);
 sems->cli_prx->empty = semaphore_create("sem_cli_prx_empty",data->buffers_size);
 sems->cli_prx->mutex = semaphore_create("sem_cli_prx_mutex",1);
 sems->prx_srv->full = semaphore_create("sem_prx_srv_full",0);
 sems->prx_srv->empty = semaphore_create("sem_prx_srv_empty",data->buffers_size);
 sems->prx_srv->mutex = semaphore_create("sem_prx_srv_mutex",1);
 sems->srv_cli->full = semaphore_create("sem_srv_cli_full",0);
 sems->srv_cli->empty = semaphore_create("sem_srv_cli_empty",data->buffers_size);
 sems->srv_cli->mutex = semaphore_create("sem_srv_cli_mutex",1);
 sems->results_mutex = semaphore_create("sem_results_mutex",1);
}

void launch_processes(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems) {

    /*Clientes*/
    for(int i = 0; i < data->n_clients;i++) {
        data->client_pids[i] = launch_process(*(data->client_pids + i),0,buffers,data,sems); 
    }

    /*Proxy*/
    for(int i = 0; i < data->n_proxies;i++) {
        data->proxy_pids[i] = launch_process(*(data->proxy_pids + i),1,buffers,data,sems);
    }

    /*Server*/
    for(int i = 0; i < data->n_servers;i++) {
        data->server_pids[i] = launch_process(*(data->server_pids + i),2,buffers,data,sems);
    }
   
}

void user_interaction(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){


    int nr_op=0;
    while (1){
        char interacao[20];
        scanf("%s",interacao);

        if(strcmp(interacao, "op")==0){
            logT(data->log_filename, interacao, NULL);
            create_request(&nr_op,buffers,data,sems);

        }else if(strcmp(interacao, "read")==0) {
            read_answer(data,sems);

        }else if(strcmp(interacao, "stop")==0){
            logT(data->log_filename, interacao, NULL);
            stop_execution(data,buffers,sems);
            
        }else if(strcmp(interacao, "help")==0){
            logT(data->log_filename, interacao, NULL);
            printf("The op option creates a new operation\n");
            printf("The read option checks the status of the operation\n");
            printf("The stop option ends the program\n");

        }else{
            logT(data->log_filename, interacao, NULL);
            perror("There is no interaction with that name\n");
        }
    }
    
}

void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){ 
 if(*op_counter < data->max_ops){
     struct operation new_op;
     getTime(&new_op.start_time); //get time for when request was made
     new_op.id =  *op_counter;
     produce_begin(sems->main_cli);

     write_rnd_access_buffer(buffers->main_cli,data->buffers_size,&new_op);

     produce_end(sems->main_cli);
     printf("Operation ID: %d \n", new_op.id);
     (*op_counter)++;
   }
}

void read_answer(struct main_data* data, struct semaphores* sems){
    int id;
    scanf("%d", &id);
    logT(data->log_filename, "read ", &id);
    semaphore_mutex_lock(sems->results_mutex);
    for(int i=0; i<data->max_ops; i++){
        struct operation* holder = data->results+i;
        if(holder->id == id){
            printf("Status da operacao: %d\n", holder->status);
            printf("Processo Cliente: %d\n", holder->client);
            printf("Processo Proxy: %d\n", holder->proxy);
            printf("Processo Servidor: %d\n", holder->server);
            
            semaphore_mutex_unlock(sems->results_mutex);
            return;
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
    
    
    /*Clientes*/
    for(int i = 0; i < data->n_clients;i++) {
        produce_end(sems->main_cli);
        produce_end(sems->cli_prx);
    }

    /*Proxy*/
    for(int i = 0; i < data->n_proxies;i++) {
        produce_end(sems->prx_srv);
    }

    /*Server*/
    for(int i = 0; i < data->n_servers;i++) {
        produce_end(sems->srv_cli);
    }
}

void wait_processes(struct main_data* data) {
    /*Clientes*/
    for(int i = 0; i < data->n_clients;i++) {
        wait_process(data->client_pids[i]);
    }

    /*Proxy*/
    for(int i = 0; i < data->n_proxies;i++) {
        wait_process(data->proxy_pids[i]);
    }

    /*Server*/
    for(int i = 0; i < data->n_servers;i++) {
        wait_process(data->server_pids[i]);
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

 destroy_dynamic_memory(data->log_filename);
 destroy_dynamic_memory(data->statistics_filename);
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
 semaphore_destroy(STR_SEM_MAIN_CLI_FULL ,sems->main_cli->full);
 semaphore_destroy(STR_SEM_MAIN_CLI_EMPTY, sems->main_cli->empty);
 semaphore_destroy(STR_SEM_MAIN_CLI_MUTEX, sems->main_cli->mutex);
 semaphore_destroy(STR_SEM_CLI_PRX_FULL, sems->cli_prx->full);
 semaphore_destroy(STR_SEM_CLI_PRX_EMPTY, sems->cli_prx->empty);
 semaphore_destroy(STR_SEM_CLI_PRX_MUTEX, sems->cli_prx->mutex);
 semaphore_destroy(STR_SEM_PRX_SRV_FULL, sems->prx_srv->full);
 semaphore_destroy(STR_SEM_PRX_SRV_EMPTY, sems->prx_srv->empty);
 semaphore_destroy(STR_SEM_PRX_SRV_MUTEX, sems->prx_srv->mutex);
 semaphore_destroy(STR_SEM_SRV_CLI_FULL, sems->srv_cli->full);
 semaphore_destroy(STR_SEM_SRV_CLI_EMPTY, sems->srv_cli->empty);
 semaphore_destroy(STR_SEM_SRV_CLI_MUTEX, sems->srv_cli->mutex);
 semaphore_destroy(STR_SEM_RESULTS_MUTEX, sems->results_mutex);
}


int main(int argc, char** argv){                 
    //init data structures
    struct main_data* data = create_dynamic_memory(sizeof(struct main_data));
    struct communication_buffers* buffers = create_dynamic_memory(sizeof(struct communication_buffers));
    buffers->main_cli = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    buffers->cli_prx = create_dynamic_memory(sizeof(struct circular_buffer));
    buffers->prx_srv = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    buffers->srv_cli = create_dynamic_memory(sizeof(struct circular_buffer));
    struct semaphores* sems = create_dynamic_memory(sizeof(struct semaphores));
    sems->main_cli = create_dynamic_memory(sizeof(struct prodcons));
    sems->cli_prx = create_dynamic_memory(sizeof(struct prodcons));
    sems->prx_srv = create_dynamic_memory(sizeof(struct prodcons));
    sems->srv_cli = create_dynamic_memory(sizeof(struct prodcons));
    alarm(data->alarm_time);
    
    g_data = data;
    g_buffers = buffers;
    g_sems = sems;
    
    //Prepare signals
    signal(SIGINT, sighandler); //set the behavior of SIGINT to sighandler
    
    struct itimerval val;
    signal(SIGALRM, sinal_horario);
    val.it_interval.tv_sec = data->alarm_time;
    val.it_interval.tv_usec = 0;
    val.it_value.tv_sec = data->alarm_time;
    val.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &val, NULL);


    //execute main code
    main_args(argc, argv, data);
    create_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, buffers);
    create_semaphores(data, sems);
    launch_processes(buffers, data, sems);
    user_interaction(buffers, data, sems);
    //release final memory
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
