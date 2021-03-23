OBJDIR = obj
CC = gcc
HEADERDIR = include
BINDIR = bin
SRCDIR = src

memory.o: $(SRCDIR)/memory.c $(HEADERDIR)/memory.h $(HEADERDIR)/memory-private.h
	$(CC) -lrt -c $(SRCDIR)/memory.c -o $(OBJDIR)/memory.o

synchronization.o: $(SRCDIR)/synchronization.c $(HEADERDIR)/synchronization.h
	$(CC) -lrt -c $(SRCDIR)/synchronization.c -o $(OBJDIR)/synchronization.o
	