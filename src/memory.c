#include "../include/memory.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Returns -1 if the allocation fails
void* create_shared_memory(char* name, int size){
    int desc = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(desc == -1){
        perror("Couldnt create shared memory");
        exit(-1);
    }
    if(ftruncate(desc, size) == -1){
        perror("Couldnt create shared memory - ftruncate failed");
        exit(-1);
    }
    void* holder = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
    if(holder == MAP_FAILED){
        perror("Couldnt create shared memory - mmap failed");
        exit(-1);
    }
    return holder;
}

void destroy_shared_memory(char* name, void* ptr, int size){
    if(munmap(ptr, size) == -1){
        perror("Couldnt destroy shared memory");
        exit(-1);
    }

    if(shm_unlink(name) == -1){
        perror("Couldnt unlink shared memory");
        exit(-1); 
    }
}

// Return NULL if no memory was allocated
void* create_dynamic_memory(int size){
    void* ptr = malloc(size);
    if(ptr == NULL){
        perror("Couldnt create dynamic memory");
        exit(-1);
    }
    memset(ptr, 0, size);
    return ptr;
}

void destroy_dynamic_memory(void* ptr){
    free(ptr);
}


void write_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    for(int i=0; i<buffer_size; i++){
        if(buffer->buffer[i].flag == 0){
            buffer->buffer[i].elem = *op;  
            buffer->buffer[i].flag = 1;
            return;
        }
    }
}

void read_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    for(int i=0; i<buffer_size; i++){
        if(buffer->buffer[i].flag == 1){
            op = &(buffer->buffer[i].elem);  
            buffer->buffer[i].flag = 0;
            return;
        }
    }
}

void write_circular_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    buffer->elems[buffer->write] = *op;
    buffer->write = (buffer->write+1) % buffer_size;
}


void read_circular_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    op = buffer->elems + buffer->read;
    buffer->read = (buffer->read+1) % buffer_size;
}