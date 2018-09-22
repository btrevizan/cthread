CC=gcc
LIB_DIR=../lib/
INC_DIR=../include/
BIN_DIR=../bin/
SRC_DIR=../src/

EXECUTABLES= ccreate_test cyield_test csetprio_test cjoin_test \
             csem_init_test cwait_test csignal_test termino_test \
		         cidentify_test

all: $(EXECUTABLES)

teste.o: teste.c teste.h
	$(CC) -std=c99 -c teste.c -Wall

ccreate_test: ccreate_test.c teste.o teste.h $(INC_DIR)cthread.h \
              $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o ccreate_test ccreate_test.c teste.o -L$(LIB_DIR) -lcthread \
	 -Wall

cyield_test: cyield_test.c teste.o teste.h $(INC_DIR)cthread.h \
             $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o cyield_test cyield_test.c teste.o -L$(LIB_DIR) -lcthread \
	-Wall

csetprio_test: csetprio_test.c teste.o teste.h $(INC_DIR)cthread.h \
               $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o csetprio_test csetprio_test.c teste.o -L$(LIB_DIR) \
	-lcthread -Wall

cjoin_test: cjoin_test.c teste.o teste.h $(INC_DIR)cthread.h \
            $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o cjoin_test cjoin_test.c teste.o -L$(LIB_DIR) -lcthread -Wall

csem_init_test: csem_init_test.c teste.o teste.h $(INC_DIR)cthread.h \
                $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o csem_init_test csem_init_test.c teste.o -L$(LIB_DIR) \
	-lcthread -Wall

cwait_test: cwait_test.c teste.o teste.h $(INC_DIR)cthread.h \
            $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o cwait_test cwait_test.c teste.o -L$(LIB_DIR) -lcthread -Wall

csignal_test: csignal_test.c teste.o teste.h $(INC_DIR)cthread.h \
              $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o csignal_test csignal_test.c teste.o -L$(LIB_DIR) -lcthread \
	-Wall

cidentify_test: cidentify_test.c teste.o teste.h $(INC_DIR)cthread.h \
                $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o cidentify_test cidentify_test.c teste.o -L$(LIB_DIR) \
	-lcthread -Wall

termino_test: termino_test.c teste.o teste.h $(INC_DIR)cthread.h \
              $(LIB_DIR)libcthread.a
	$(CC) -std=c99 -o termino_test termino_test.c teste.o -L$(LIB_DIR) -lcthread \
	-Wall

run: all
	for exec in $(EXECUTABLES); do ./$$exec; done

clean:
	rm -rf $(EXECUTABLES) teste.o