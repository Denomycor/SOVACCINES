OBJDIR = obj
CC = gcc
HEADERDIR = include
BINDIR = bin
SRCDIR = src

memory.o: $(SRCDIR)/memory.c $(HEADERDIR)/memory.h $(HEADERDIR)/memory-private.h
	$(CC) -lrt -c $(SRCDIR)/memory.c -o $(OBJDIR)/memory.o

synchronization.o: $(SRCDIR)/synchronization.c $(HEADERDIR)/synchronization.h
	$(CC) -lrt -c $(SRCDIR)/synchronization.c -o $(OBJDIR)/synchronization.o

client.o: $(SRCDIR)/client.c $(HEADERDIR)/client.h
	$(CC)  -c $(SRCDIR)/client.c -o $(OBJDIR)/client.o

proxy.o: $(SRCDIR)/proxy.c $(HEADERDIR)/proxy.h
	$(CC) -c $(SRCDIR)/proxy.c -o $(OBJDIR)/proxy.o

server.o: $(SRCDIR)/server.c $(HEADERDIR)/server.h
	$(CC) -c $(SRCDIR)/server.c -o $(OBJDIR)/server.o

process.o: $(SRCDIR)/process.c $(HEADERDIR)/process.h $(HEADERDIR)/server.h $(HEADERDIR)/proxy.h $(HEADERDIR)/client.h
	$(CC) -c $(SRCDIR)/process.c -o $(OBJDIR)/process.o

main.o: $(SRCDIR)/main.c $(HEADERDIR)/main.h $(HEADERDIR)/memory.h
	$(CC) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o