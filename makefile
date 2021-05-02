




FLAGS = -w -lm -lrt -lpthread -pthread
CC = gcc
OBJDIR = obj
OBJFILES = main.o proxy.o  client.o server.o  process.o  memory.o  synchronization.o  stats.o  configuration.o  sotime.o  log.o  sosignal.o
HEADERDIR = include 
SRCDIRDIR = src
BINDIR = bin


proxy.o = $(HEADERDIR)/main.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/proxy.h $(HEADERDIR)/memory.h $(HEADERDIR)/sotime.h	
memory.o = $(HEADERDIR)/memory-private.h $(HEADERDIR)/memory.h
process.o = $(HEADERDIR)/client.h $(HEADERDIR)/main.h $(HEADERDIR)/memory.h $(HEADERDIR)/proxy.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/server.h 
synchronization.o = $(HEADERDIR)/synchronization.h
client.o = $(HEADERDIR)/client.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/main.h $(HEADERDIR)/memory.h $(HEADERDIR)/sotime.h
server.o = $(HEADERDIR)/main.h $(HEADERDIR)/server.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/memory.h $(HEADERDIR)/sotime.h
		
main.o =  $(HEADERDIR)/main.h $(HEADERDIR)/client.h $(HEADERDIR)/server.h $(HEADERDIR)/proxy.h $(HEADERDIR)/process.h $(HEADERDIR)/synchronization.h $(HEADERDIR)/memory.h $(HEADERDIR)/memory-private.h $(HEADERDIR)/stats.h $(HEADERDIR)/configuration.h $(HEADERDIR)/sotime.h $(HEADERDIR)/log.h $(HEADERDIR)/sosignal.h

stats.o = $(HEADERDIR)/stats.h $(HEADERDIR)/main.h $(HEADERDIR)/configuration.h $(HEADERDIR)/sotime.h $(HEADERDIR)/log.h
configuration.o = $(HEADERDIR)/configuration.h
sotime.o = $(HEADERDIR)/sotime.h
log.o = $(HEADERDIR)/log.h $(HEADERDIR)/sotime.h $(HEADERDIR)/configuration.h
sosignal.o = $(HEADERDIR)/main.h $(HEADERDIR)/sotime.h $(HEADERDIR)/sosignal.h $(HEADERDIR)/synchronization.h


sovaccines: $(OBJFILES)
	$(CC) $(addprefix $(OBJDIR)/,$(OBJFILES))  $(FLAGS) -g -o $(addprefix ./bin/,$@)

%.o: src/%.c $($@)
	$(CC) -I include -o $(addprefix obj/,$@) -g -c $<

clean:
	rm $(addprefix $(OBJDIR)/,$(OBJFILES))
	rm $(BINDIR)/sovaccines
