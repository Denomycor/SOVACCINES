#include "stats.h"
#include <stdio.h>

void writeStatisticsFile(FILE *stream,struct main_data* data,int i){
    writeProcessStats(stream,data,i);
    writeNumberOP(stream,data->op[i],i);
    writeStatus(stream,data->op[i],i);
    writeIds(stream,data->op[i],i);
    writeLocalsTime(stream,data->op[i],i);

}

void writeProcessStats(FILE *stream,struct main_data* data, int i) {
    fputs("Process Statistics:\n",stream);
    char* client;
    char* proxy;
    char* server;
    asprintf(&client,"Client %d received %d requests\n",data->client_pids[i],data->client_stats[i]);
    asprintf(&proxy,"Client %d received %d requests\n",data->proxy_pids[i],data->proxy_stats[i]);
    asprintf(&server,"Client %d received %d requests\n",data->server_pids[i],data->server_stats[i]);
    fputs(&client,stream);
    fputs(&proxy,stream);
    fputs(&server,stream);
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
