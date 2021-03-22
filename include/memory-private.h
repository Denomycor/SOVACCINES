#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD

#include<stdlib.h>

//Defines the buffer size for buffers
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif

//Defenition of operation struct, so this file can see it
struct operation; //memory.h

//Defines the elements to be saved in the buffers
typedef buffer_elem operation;


struct circular_buffer { 	
    
};


struct rnd_access_buffer { 		
    int flags[BUFFER_SIZE];
    buffer_elem* elems[BUFFER_SIZE];

};


void rnd_access_buffer_clear(struct rnd_access_buffer* o){
    memset(o->flags, 0, sizeof(int)*BUFFER_SIZE);
}



#endif
