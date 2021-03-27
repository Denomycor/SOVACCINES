#include <stdlib.h>
#include <client.h>
#include <server.h>
#include <proxy.h>

int launch_process(int process_id, int process_code, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
 int pid;
 if ((pid = fork()) == -1){
     perror("An error has occured");
     exit(1);
 }
 if (pid == 0){     
     int x; 
    if (process_code == 0) {
       x = execute_client(process_id,buffers,data,sems);
       exit(x);
    }
    if (process_code == 1) {
       x = execute_proxy(process_id,buffers,data,sems);
       exit(x);
    }

    if (process_code == 2) {
       x = execute_server(process_id,buffers,data,sems);
       exit(x);
    }
 } else {
     return pid;
 }
}

int wait_process(int process_id){
 int* status;   
 waitpid(process_id,&status,0);
 if (WIFEXITED(status)){
     return WEXITSTATUS(status);
 }
}

