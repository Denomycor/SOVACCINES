FLAGS = -w -lm -pthread -lrt -lpthread
CC = gcc
OBJDIR = obj
OBJFILES = main.o client.o proxy.o server.o synchronization.o  process.o  memory.o
HEADERDIR = include
SRCDIRDIR = src
BINDIR = bin

memory.o = ./$(HEASERDIR)/memory-private.h ./$(HEASERDIR)/memory.h
synchronization.o = ./$(HEASERDIR)/synchronization.h
server.o = ./$(HEASERDIR)/main.h ./$(HEASERDIR)/memory.h ./$(HEASERDIR)/server.h ./$(HEASERDIR)/synchronization.h
main.o =  ./$(HEASERDIR)/main.h ./$(HEASERDIR)/client.h ./$(HEASERDIR)/memory.h ./$(HEASERDIR)/server.h ./$(HEASERDIR)/proxy.h ./$(HEASERDIR)/process.h ./$(HEASERDIR)/synchronization.h ./$(HEASERDIR)/memory-private.h
client.o = ./$(HEASERDIR)/client.h ./$(HEASERDIR)/main.h ./$(HEASERDIR)/memory.h ./$(HEASERDIR)/synchronization.h
proxy.o = ./$(HEASERDIR)/main.h ./$(HEASERDIR)/proxy.h ./$(HEASERDIR)/memory.h ./$(HEASERDIR)/synchronization.h
process.o = ./$(HEASERDIR)/main.h ./$(HEASERDIR)/client.h ./$(HEASERDIR)/memory.h ./$(HEASERDIR)/server.h ./$(HEASERDIR)/proxy.h ./$(HEASERDIR)/synchronization.h



sovaccines: $(OBJFILES)
	$(CC) $(addprefix $(OBJDIR)/,$(OBJFILES))  $(FLAGS) -g -o $(addprefix ./bin/,$@)

%.o: ./src/%.c $($@)
	$(CC) -I include -o $(addprefix obj/,$@) -g -c $<

clean:
	rm $(addprefix $(OBJDIR)/,$(OBJFILES))
	rm ./bin/sovaccines