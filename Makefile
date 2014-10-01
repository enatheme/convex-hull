#make file for the convexh stuff
#Benjamin Teissier
#teissier@cs.ubishops.ca
CC=gcc
CF= -std=c99 
LDFLAGS= -lm 
#LDFLAGS= -lpthread
EXEC=convexh #if we want to launch an execution directly

all: convexh

convexh: main.o
	$(CC) -o convexh main.o $(LDFLAGS)

main.o: main.c main.h
	$(CC) -o main.o -c main.c $(CF)

clean: 
	rm *.o

mrproper: clean
	rm changer

