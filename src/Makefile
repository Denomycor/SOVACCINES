OBJDIR = obj
CC = gcc
HEADERDIR = include
BINDIR = bin
SRCDIR = src
FLAGS = -lpthread -w -lm -pthread -lrt

memory.o: $(SRCDIR)/memory.c $(HEADERDIR)/memory.h $(HEADERDIR)/memory-private.h
	$(CC) $(FLAGS) -c $(SRCDIR)/memory.c -g -o $(OBJDIR)/memory.o

synchronization.o: $(SRCDIR)/synchronization.c $(HEADERDIR)/synchronization.h
	$(CC) $(FLAGS) -c $(SRCDIR)/synchronization.c -g -o $(OBJDIR)/synchronization.o

client.o: $(SRCDIR)/client.c $(HEADERDIR)/client.h
	$(CC) $(FLAGS) -c $(SRCDIR)/client.c -g -o $(OBJDIR)/client.o

proxy.o: $(SRCDIR)/proxy.c $(HEADERDIR)/proxy.h
	$(CC) $(FLAGS) -c $(SRCDIR)/proxy.c -g -o $(OBJDIR)/proxy.o

server.o: $(SRCDIR)/server.c $(HEADERDIR)/server.h
	$(CC) $(FLAGS) -c $(SRCDIR)/server.c -g -o $(OBJDIR)/server.o

process.o: $(SRCDIR)/process.c $(HEADERDIR)/process.h $(HEADERDIR)/server.h $(HEADERDIR)/proxy.h $(HEADERDIR)/client.h
	$(CC) $(FLAGS) -c $(SRCDIR)/process.c -g -o $(OBJDIR)/process.o

main.o: $(SRCDIR)/main.c $(HEADERDIR)/main.h $(HEADERDIR)/memory.h
	$(CC) $(FLAGS) -c $(SRCDIR)/main.c -g -o $(OBJDIR)/main.o

sovaccines: $(OBJDIR)/memory.o $(OBJDIR)/synchronization.o $(OBJDIR)/client.o $(OBJDIR)/proxy.o $(OBJDIR)/server.o $(OBJDIR)/process.o $(OBJDIR)/main.o
	$(CC) $(FLAGS) $(OBJDIR)/memory.o $(OBJDIR)/synchronization.o $(OBJDIR)/client.o $(OBJDIR)/proxy.o $(OBJDIR)/server.o $(OBJDIR)/process.o $(OBJDIR)/main.o -g -o $(BINDIR)/sovaccines