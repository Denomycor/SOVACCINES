#include "../include/stats.h"
#include "../include/main.h"
#include "../include/log.h"
#include "../include/sotime.h"
#include "../include/configuration.h"
#include <stdio.h>
#include <time.h>

void writeStatisticsFile(const char* name,struct main_data* data){
    FILE* stream = openFile(name, "w");
    
    writeProcessStats(stream,data);
    writeOpStatistics(stream,data->results,data);

    closeFile(stream);
}

void writeProcessStats(FILE *stream,struct main_data* data) {
    fputs("Process Statistics:\n",stream);

    char temp[100];

    for(int i = 0; i < data->n_clients;i++) {
        sprintf(temp,"Client %d received %d requests\n", data->client_pids[i], data->client_stats[i]);
        fputs(temp,stream);
    }
    
    for(int i = 0; i < data->n_proxies;i++) {
        sprintf(temp,"Proxy %d received %d requests\n",data->proxy_pids[i],data->proxy_stats[i]);
        fputs(temp,stream);
    }

    for(int i = 0; i < data->n_servers;i++) {
        sprintf(temp,"Server %d received %d requests\n",data->server_pids[i],data->server_stats[i]);
        fputs(temp,stream);
    }
}

void writeOpStatistics(FILE* stream,struct operation* op, struct main_data* data) {
    int nr_processed = 0;
    for(int i = 0; i < data->n_servers; i++) {
        nr_processed += data->server_stats[i];
    }
    for(int i = 0; i < nr_processed;i++) {
        writeNumberOp(stream,op,i);
        writeStatus(stream,op,i);
        writeIds(stream,op,i);
        writeLocalsTime(stream,op,i);
    }
}

void writeNumberOp(FILE *stream,struct operation* op,int i) {
    char opStr[10];
    sprintf(opStr,"OP: %d\n",op[i].id);
    fputs(opStr,stream);
}

void writeStatus(FILE *stream,struct operation* op,int i) {
    char opStr[15];
    sprintf(opStr,"Status: %d\n",op[i].status);
    fputs(opStr,stream);
}

void writeIds(FILE *stream,struct operation* op,int i) {
    char temp[20];

    sprintf(temp,"Client_id: %d\n",op[i].client);
    fputs(temp,stream);

    sprintf(temp,"Proxy_id: %d\n",op[i].proxy);
    fputs(temp,stream);

    sprintf(temp,"Server_id: %d\n",op[i].server);
    fputs(temp,stream);

}

void writeLocalsTime(FILE* stream,struct operation* op,int i) {
    char temp[100];
    struct tm time;
    struct timespec diff;

    fputs("Created: ", stream);
    localtime_r(&op[i].start_time.tv_sec, &time);
    logTimeFormated(stream, temp, &time, op[i].start_time.tv_nsec);
    
    fputs("\nClient_time: ", stream);
    localtime_r(&op[i].client_time.tv_sec, &time);
    logTimeFormated(stream, temp, &time, op[i].client_time.tv_nsec);

    fputs("\nProxy_time: ", stream);
    localtime_r(&op[i].proxy_time.tv_sec, &time);
    logTimeFormated(stream, temp, &time, op[i].proxy_time.tv_nsec);

    fputs("\nServer_time: ", stream);
    localtime_r(&op[i].server_time.tv_sec, &time);
    logTimeFormated(stream, temp, &time, op[i].server_time.tv_nsec);

    fputs("\nEnded: ", stream);
    localtime_r(&op[i].end_time.tv_sec, &time);
    logTimeFormated(stream, temp, &time, op[i].end_time.tv_nsec);

    timespec_diff(&op[i].start_time, &op[i].end_time, &diff);
    fputs("\nTotal Time: ", stream);

    sprintf(temp, "%ld", diff.tv_sec);
    fputs(temp, stream);
    fputs(".", stream);

    sprintf(temp, "%ld", diff.tv_nsec);
    fputs(temp, stream);
    fputs("\n", stream);  

}
