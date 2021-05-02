




FLAGS = -w -lm -lrt -lpthread -pthread
CC = gcc
OBJDIR = obj
OBJFILES = main.o proxy.o  client.o server.o  process.o  memory.o  synchronization.o  stats.o  configuration.o  sotime.o  log.o  sosignal.o
HEADERDIR = include 
SRCDIRDIR = src
BINDIR = bin

#//Dar update aos  .o  seguintes caso tenham sido acrescentados includes nos seus  .c

proxy.o = $(HEADERDIR)/main.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/proxy.h $(HEADERDIR)/memory.h
memory.o = $(HEADERDIR)/memory-private.h $(HEADERDIR)/memory.h
process.o = $(HEADERDIR)/client.h $(HEADERDIR)/main.h $(HEADERDIR)/memory.h $(HEADERDIR)/proxy.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/server.h 
synchronization.o = $(HEADERDIR)/synchronization.h
client.o = $(HEADERDIR)/client.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/main.h $(HEADERDIR)/memory.h
server.o = $(HEADERDIR)/main.h $(HEADERDIR)/server.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/memory.h

main.o =  $(HEADERDIR)/main.h $(HEADERDIR)/client.h $(HEADERDIR)/server.h $(HEADERDIR)/proxy.h $(HEADERDIR)/process.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/memory.h $(HEADERDIR)/memory-private.h


#//Acrescentar os novos  .o  e escrever $(HEADERDIR)/xxxxxxxx.h para cada include que esteja no seu  .c  ou  .h (que tenha sido escrito por nos, ou seja #include "../include/xxxxx.h")

stats.o = 
configuration.o = 
sotime.o = 
log.o = 
sosignal.o = 


sovaccines: $(OBJFILES)
	$(CC) $(addprefix $(OBJDIR)/,$(OBJFILES))  $(FLAGS) -g -o $(addprefix ./bin/,$@)

%.o: src/%.c $($@)
	$(CC) -I include -o $(addprefix obj/,$@) -g -c $<

clean:
	rm $(addprefix $(OBJDIR)/,$(OBJFILES))
	rm $(BINDIR)/sovaccines