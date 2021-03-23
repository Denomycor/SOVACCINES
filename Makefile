OBJDIR = obj
CC = gcc
HEADERDIR = include
BINDIR = bin
SRCDIR = src

$(OBJDIR)/memory.o: $(SRCDIR)/memory.c $(HEADERDIR)/memory.h $(HEADERDIR)/memory-private.h
	$(CC) -lrt -c $(SRCDIR)/memory.c