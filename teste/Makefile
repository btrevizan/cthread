CC=gcc
LIB_DIR=../lib/
INC_DIR=../include/
BIN_DIR=../bin/
SRC_DIR=../src/

EXECUTABLES= ccreate_test cyield_test csetprio_test cjoin_test \
             csem_init_test cwait_test csignal_test termino_test \
		         cidentify_test

all: $(EXECUTABLES)

ccreate_test: ccreate_test.c teste.h $(INC_DIR)cthread.h $(LIB_DIR)libcthread.a
	$(CC) -o ccreate_test ccreate_test.c -L$(LIB_DIR) -lcthread -Wall

cyield_test: cyield_test.c teste.h $(INC_DIR)cthread.h $(LIB_DIR)libcthread.a
	$(CC) -o cyield_test cyield_test.c -L$(LIB_DIR) -lcthread -Wall

csetprio_test: csetprio_test.c teste.h $(INC_DIR)cthread.h \
               $(LIB_DIR)libcthread.a
	$(CC) -o csetprio_test csetprio_test.c -L$(LIB_DIR) -lcthread -Wall

cjoin_test: cjoin_test.c teste.h $(INC_DIR)cthread.h $(LIB_DIR)libcthread.a
	$(CC) -o cjoin_test cjoin_test.c -L$(LIB_DIR) -lcthread -Wall

csem_init_test: csem_init_test.c teste.h $(INC_DIR)cthread.h \
                $(LIB_DIR)libcthread.a
	$(CC) -o csem_init_test csem_init_test.c -L$(LIB_DIR) -lcthread -Wall

cwait_test: cwait_test.c teste.h $(INC_DIR)cthread.h $(LIB_DIR)libcthread.a
	$(CC) -o cwait_test cwait_test.c -L$(LIB_DIR) -lcthread -Wall

csignal_test: csignal_test.c teste.h $(INC_DIR)cthread.h $(LIB_DIR)libcthread.a
	$(CC) -o csignal_test csignal_test.c -L$(LIB_DIR) -lcthread -Wall

cidentify_test: cidentify_test.c teste.h $(INC_DIR)cthread.h \
                $(LIB_DIR)libcthread.a
	$(CC) -o cidentify_test cidentify_test.c -L$(LIB_DIR) -lcthread -Wall

termino_test: termino_test.c teste.h $(INC_DIR)cthread.h $(LIB_DIR)libcthread.a
	$(CC) -o termino_test termino_test.c -L$(LIB_DIR) -lcthread -Wall

run: all
	for exec in $(EXECUTABLES); do ./$$exec; done

clean:
	rm -rf $(EXECUTABLES)