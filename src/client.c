/**
Grupo054
Afonso Esteves, fc54394
Diogo Fernandes, fc54458
Miguel Santos, fc54461
*/

#include "../include/client.h"
#include <stdio.h>
#include "../include/sotime.h"

int execute_client(int client_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
 while(1){

    struct operation* op;
    client_get_operation(op,buffers,data,sems);
    
    if((op->id) != -1 && *(data->terminate) == 0){
        client_process_operation(op,client_id,data->client_stats);
        client_send_operation(op,buffers,data,sems);
    }

    client_receive_answer(op,buffers,data,sems);
    if((op->id) != -1 && *(data->terminate) == 0){
        client_process_answer(op,data,sems);
    }

    if(*(data->terminate) == 1) {
        return *(data->client_stats);
    }
 }
}

void client_get_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
 consume_begin(sems->main_cli);
 if(*(data->terminate) == 1){
        return;
 }
 read_rnd_access_buffer(buffers->main_cli,data->buffers_size,op);
 consume_end(sems->main_cli);
}

void client_process_operation(struct operation* op, int cient_id, int* counter){
 getTime(&op->client_time); //get time for when client proccess this op
 op->client = cient_id;
 op->status = 'C';
 (*counter)++;
}

void client_send_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
 produce_begin(sems->cli_prx);
 write_circular_buffer(buffers->cli_prx,data->buffers_size,op);
 produce_end(sems->cli_prx);
}

void client_receive_answer(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
 consume_begin(sems->srv_cli);
 
 if(*(data->terminate) == 1){
     return;
 }
 read_circular_buffer(buffers->srv_cli,data->buffers_size,op);

 consume_end(sems->srv_cli);
}

void client_process_answer(struct operation* op, struct main_data* data, struct semaphores* sems){
 semaphore_mutex_lock(sems->results_mutex);
 
 data->results[data->results->id] = *op;  //guarda operacao finalizada ao data
 (data->results->id)++;                   //incrementa-se o id do results para que se possa processar uma nova op
 getTime(&op->end_time);                  //get time for when op was finished

 semaphore_mutex_unlock(sems->results_mutex);
 printf("operation ended\n");
}
