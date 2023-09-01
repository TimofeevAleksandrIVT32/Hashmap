CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

OBJS = main.o hash.o

all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o res

main.o: main.c hash.h
	$(CC) $(CFLAGS) -c main.c

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c hash.c

clean:
	rm -f $(OBJS) res
