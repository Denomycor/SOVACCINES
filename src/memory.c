#include "../include/memory.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void* create_shared_memory(char* name, int size){
    int desc = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(desc == -1 || ftruncate(desc, size)==-1){
        return (void*)-1;
    }
    int*const handler = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
    if(handler == MAP_FAILED){
        return (void*)-1;
    }
    return handler;
}

void destroy_shared_memory(char* name, void* ptr, int size){
    int ret = munmap(ptr, size);
    if(ret == -1 || shm_unlink(name)==-1){
        exit(-1);
    }
}

void* create_dynamic_memory(int size){
    void* ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

void destroy_dynamic_memory(void* ptr){
    free(ptr);
}


void write_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    for(int i=0; i<buffer_size; i++){
        if(buffer->flags[i] == 0){
            buffer->elems[i] = op;  
            buffer->flags[i] = 1;
            return;
        }
    }
}

void read_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    for(int i=0; i<buffer_size; i++){
        if(buffer->flags[i] == 1){
            op = buffer->elems[i];
            buffer->flags[i] = 0;
            return;
        }
    }
}

void write_circular_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    buffer->elems[buffer->write] = op;
    buffer->write = (buffer->write+1) % buffer_size;
}


void read_circular_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    op = buffer->elems[buffer->read];
    buffer->read = (buffer->read+1) % buffer_size;
}