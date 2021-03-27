#include "process.h"
#include "client.c"
#include "proxy.c"
#include "server.c"
#include <sys/types.h>
#include <sys/wait.h>

int launch_process(int process_id, int process_code, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    int pid; 
    if((pid = fork())== -1) {
        perror("erro");
        exit(-1);
    }
    if(pid == 0) {
        switch (process_code)
        {
        case '0':
            exit(execute_client(data.n_clients,buffers,data,sems));
            break;
        case '1':
            exit(execute_proxy(data.n_proxies,buffers,data,sems));
            break;
        case '2':
            exit(execute_server(data.n_servers,buffers,data,sems));
            break;
        default:
            perror("Process_code is wrong");
        }
    }
    else {
        return pid;
    }

}

int wait_process(int process_id) {
    int *status_process;
    waitpid(process_id,*status_process,0);
    if(WIFEXITED(status_process)) {
        return status_process;
    }
    else {
        perror("Process ended bad");
        exit(-1);
    }
}