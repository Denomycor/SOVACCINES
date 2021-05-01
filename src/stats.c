#include "stats.h"
#include <stdio.h>
#include <time.h>

void writeStatisticsFile(FILE *stream,struct main_data* data){
    writeProcessStats(stream,data);
    writeOpStatistics(FILE *stream,data -> results);
}

void writeProcessStats(FILE *stream,struct main_data* data) {
    fputs("Process Statistics:\n",stream);
    for(int i = 0; i < data ->n_clients,i++) {
        char* client;
        asprintf(&client,"Client %d received %d requests\n",
                data->client_pids[i],  
                data->client_stats[i]);
        fputs(&client,stream);
    }
    
    for(int i = 0; i < data ->n_proxies,i++) {
        char* proxy;
        asprintf(&proxy,"Proxy %d received %d requests\n",data->proxy_pids[i],data->proxy_stats[i]);
        fputs(&proxy,stream);
    }

    for(int i = 0; i < data ->n_servers,i++) {
        char* server;
        asprintf(&server,"Server %d received %d requests\n",data->server_pids[i],data->server_stats[i]);
        fputs(&server,stream);
    }
}

void writeOpStatistics(FILE *stream,struct operation* op,struct main_data* data) {
    int nr_processed = 0;
     for(int i = 0; i< data->n_servers; i++) {
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
    char* opStr;
    asprintf(&opStr,"OP:%d\n",op[i]->id);
    fputs(&opStr,stream);
}

void writeStatus(FILE *stream,struct operation* op,int i) {
    char* opStr;
    asprintf(&opStr,"OP:%d\n",op[i]->status);
    fputs(&opStr,stream);
}

void writeIds(FILE *stream,struct operation* op,int i) {
    char* clientId;
    char* proxyId;
    char* serverId;
    asprintf(&clientId,"Client_id:%d\n",op[i]->client);
    asprintf(&proxyId,"Client_id:%d\n",op[i]->proxy);
    asprintf(&serverId,"Client_id:%d\n",op[i]->server);
    fputs(&clientId,stream);
    fputs(&proxyId,stream);
    fputs(&serverId,stream);
}

void writeLocalsTime(FILE *stream,struct operation* op,int i) {
    char* createdTime;
    char* clientTime;
    char* proxyTime;
    char* serverTime; 
    char* ended;
    char* totalTime;
    asprintf(&createdTime,"Created:%s\n",ctime(op[i]->start_time->tv_sec));
    asprintf(&clientTime,"Client_time:%s\n",ctime(op[i]->client_time->tv_sec));
    asprintf(&proxyTime,"Porxy_time:%s\n",ctime(op[i]->proxy_time->tv_sec));
    asprintf(&serverTime,"Server_time:%s\n",ctime(op[i]->server_time->tv_sec));
    asprintf(&ended,"Ended:%s\n",ctime(op[i]->end_time->tv_sec));
    asprintf(&totalTime,"Total Time:%s\n",ctime(op[i]->end_time->tv_sec - op[i]->start_time->tv_sec));
    fputs(&createdTime,stream);
    fputs(&clientTime,stream);
    fputs(&proxyTime,stream);
    fputs(&serverTime,stream);
    fputs(&ended,stream);
    fputs(&totalTime,stream);
    fputs("\n",stream);

}
