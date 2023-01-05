CC = gcc
CFLAGS = -Wall -std=c99 

all: run

run: carDealership.c 
	$(CC) $(CFLAGS) -o carDealership carDealership.c

clean:
	rm -f carDealership