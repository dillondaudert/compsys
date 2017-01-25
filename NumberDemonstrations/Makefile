CC=gcc
CFLAGS=-std=c99 -Wall
SRC=problems.c
OBJ=problems.o
OUT=problems.out

all: outputs

outputs: objs
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT)

objs:
	$(CC) -c $(SRC) 

clean:
	rm *.o

clean-all:
	rm *.o *.out
