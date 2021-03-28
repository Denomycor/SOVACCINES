#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD

#include "memory.h"


struct circular_buffer { 	
    struct operation* elems;
    int read;
    int write;
};




struct rnd_access_buffer { 		
    struct operation* elems;
    int* flags;
};


#endif
