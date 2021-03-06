/**
Grupo054
Afonso Esteves, fc54394
Diogo Fernandes, fc54458
Miguel Santos, fc54461
*/

#include "../include/server.h"
#include "../include/sotime.h"
#include <unistd.h>

int execute_server(int server_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
 while(1){

    struct operation op;
    server_receive_operation(&op,buffers,data,sems);

    if((op.id) != -1 && *(data->terminate) == 0){
        int j=0;
        for(; j<data->n_servers;j++){
            if(data->server_pids[j] == getpid()){
                break;
            }
        }
        server_process_operation(&op,server_id,data->server_stats+j);
        server_send_answer(&op,buffers,data,sems);
    }

    if(*(data->terminate) == 1){
        return *(data->server_stats);
    }    
 }
}

void server_receive_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
 consume_begin(sems->prx_srv);

 if(*(data->terminate) == 1){
     return;
 }
 
 read_rnd_access_buffer(buffers->prx_srv,data->buffers_size,op);
 consume_end(sems->prx_srv);
}

void server_process_operation(struct operation* op, int server_id, int* counter){
 getTime(&op->server_time); //get time for when server proccess this op
 op->server = server_id;
 op->status = 'S';
 (*counter)++;
}

void server_send_answer(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
 produce_begin(sems->srv_cli);
 write_circular_buffer(buffers->srv_cli,data->buffers_size,op);
 produce_end(sems->srv_cli);

}
