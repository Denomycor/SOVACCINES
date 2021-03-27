#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD

#include<stdlib.h>

typedef struct operation buffer_elem;

#define BUFFER_SIZE 10


struct circular_buffer { 	
    buffer_elem* elems[BUFFER_SIZE];
    int read;
    int write;
};


struct rnd_access_buffer { 		
    int flags[BUFFER_SIZE]; 
    buffer_elem* elems[BUFFER_SIZE];
};


#endif
