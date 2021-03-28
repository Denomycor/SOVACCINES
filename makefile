OBJETOS = main.o client.o proxy.o server.o synchronization.o  process.o  memory.o
CC = gcc
OBJ_DIR = obj
FLAGS = -w -lm -pthread -lrt -lpthread

main.o =  ./include/main.h ./include/client.h ./include/memory.h ./include/server.h ./include/proxy.h ./include/process.h ./include/synchronization.h ./include/memory-private.h
proxy.o = ./include/main.h ./include/proxy.h ./include/memory.h ./include/synchronization.h
client.o = ./include/client.h ./include/main.h ./include/memory.h ./include/synchronization.h
server.o = ./include/main.h ./include/memory.h ./include/server.h ./include/synchronization.h
process.o = ./include/main.h ./include/client.h ./include/memory.h ./include/server.h ./include/proxy.h ./include/synchronization.h
synchronization.o = ./include/synchronization.h
memory.o = ./include/memory-private.h ./include/memory.h


sovaccines: $(OBJETOS)
	$(CC) $(addprefix $(OBJ_DIR)/,$(OBJETOS))  $(FLAGS) -g -o $(addprefix ./bin/,$@)

%.o: ./src/%.c $($@)
	$(CC) -I include -o $(addprefix $(OBJ_DIR)/,$@) -g -c $<

clean:
	rm $(addprefix $(OBJ_DIR)/,$(OBJETOS))
	rm ./bin/sovaccines