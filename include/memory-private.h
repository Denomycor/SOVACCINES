#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD

#include<stdlib.h>

typedef struct operation buffer_elem;

struct pair{
    int flag;
    buffer_elem elem;
};

struct circular_buffer { 	
    buffer_elem* elems;
    int read;
    int write;
};


struct rnd_access_buffer { 		
    struct pair* buffer;
};


#endif
