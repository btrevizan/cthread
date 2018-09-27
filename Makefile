CC=gcc
LIB_DIR=./lib/
INC_DIR=./include/
BIN_DIR=./bin/
SRC_DIR=./src/

all: libcthread.a

libcthread.a: lib.o $(BIN_DIR)support.o
	ar crs $(LIB_DIR)libcthread.a $(BIN_DIR)lib.o $(BIN_DIR)support.o

lib.o: $(SRC_DIR)lib.c $(INC_DIR)cdata.h $(INC_DIR)cthread.h \
       $(INC_DIR)support.h $(INC_DIR)helpers.h
	$(CC) -c -o $(BIN_DIR)lib.o $(SRC_DIR)lib.c -Wall

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/lib.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~