#include "../include/synchronization.h"
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>


sem_t * semaphore_create(char* name, int value){
    sem_t* holder = sem_open(name, O_CREAT, 0xFFFFFFFF, value);
    if(holder == SEM_FAILED){
        exit(-1);
    }
    return holder;
}

void semaphore_destroy(char* name, sem_t* semaphore){
    if(sem_close(semaphore) == -1){
        exit(-1);
    }
    if(sem_unlink(name) == -1){
        exit(-1);
    }
}

void semaphore_mutex_lock(sem_t* sem){
    sem_wait(sem);
}

void semaphore_mutex_unlock(sem_t* sem){
    sem_post(sem);
}