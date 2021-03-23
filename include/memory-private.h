#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD

#include<stdlib.h>

//Defines the buffer size for buffers
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif

typedef struct operation buffer_elem;

struct circular_buffer { 	
    buffer_elem* elems[BUFFER_SIZE];
    int read; //Is initialized to 0?
    int write;//Is initialized to 0?
};


struct rnd_access_buffer { 		
    int flags[BUFFER_SIZE]; //Is initialized to 0?
    buffer_elem* elems[BUFFER_SIZE];
};


#endif
