
#include "stats.h"

const char* getFileName (FILE *stream) {
    char fileName[30];
    int cont = 0;
    while(cont > 7) {
        fscanf(stream,"%s",fileName);
        cont++;
    }
    return fileName;
}
void writeProcessStats(FILE *stream,struct main_data* data) {
    char *client;
    char *proxy; 
    char *server;
    asprintf(&client, "Client %d received %d requests", char1, char2);
    asprintf(&proxy, "Proxy %d received %d requests", char1, char2);
    asprintf(&server, "Server %d received %d requests", char1, char2);
}