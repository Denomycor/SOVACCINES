/**
Grupo054
Afonso Esteves, fc54394
Diogo Fernandes, fc54458
Miguel Santos, fc54461
*/

#include "../include/synchronization.h"
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>


sem_t * semaphore_create(char* name, int value){
    sem_t* holder = sem_open(name, O_CREAT, 0xFFFFFFFF, value);
    if(holder == SEM_FAILED){
        perror("Couldnt create semaphore");
        exit(-1);
    }
    return holder;
}


void semaphore_destroy(char* name, sem_t* semaphore){
    if(sem_close(semaphore) == -1){
        perror("Couldnt close semaphore");
        exit(-1);
    }
    if(sem_unlink(name) == -1){
        perror("Couldnt unlink semaphore");
        exit(-1);
    }
}


void semaphore_mutex_lock(sem_t* sem){
    if(sem_wait(sem)!=0){
        perror("Couldnt lock semaphore");
        exit(-1);
    }
}


void semaphore_mutex_unlock(sem_t* sem){
    if(sem_post(sem)!=0){
        perror("Couldnt unlock semaphore");
        exit(-1);
    }
}


void produce_begin(struct prodcons* pc){
    semaphore_mutex_lock(pc->empty);
    semaphore_mutex_lock(pc->mutex);
}


void produce_end(struct prodcons* pc){
    semaphore_mutex_unlock(pc->mutex);
    semaphore_mutex_unlock(pc->empty);
}


void consume_begin(struct prodcons* pc){
    semaphore_mutex_lock(pc->full);
    semaphore_mutex_lock(pc->mutex);
}


void consume_end(struct prodcons* pc){
    semaphore_mutex_unlock(pc->mutex);
    semaphore_mutex_unlock(pc->full);
}
