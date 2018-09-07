CC=gcc
LIB_DIR=../lib/
INC_DIR=../include/
BIN_DIR=../bin/
SRC_DIR=../src/

all: ccreate_test.o cyield_test.o

ccreate_test.o: ccreate_test.c teste.h $(INC_DIR)cthread.h
	$(CC) -c ccreate_test.c -Wall

cyield_test.o: cyield_test.c teste.h $(INC_DIR)cthread.h
	$(CC) -c cyield_test.c -Wall

clean:
	rm -rf *.o