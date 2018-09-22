#include "../include/cthread.h"
#include "teste.h"
#include <string.h>
#include <stdlib.h>

typedef struct args_t {
    char* trace;
    char number_str[2];
    int cjoin_tid_index;
    char number_str_after_unblock[2];
    int prio;
    int* tids;
} args_t;

void* func(void* arg) {
    args_t* args = (args_t*) arg;

    strcat(args->trace, args->number_str);

    if (args->cjoin_tid_index >= 0) {
      cjoin(args->tids[args->cjoin_tid_index]);

      strcat(args->trace, args->number_str_after_unblock);
    }

    return NULL;
}

int main() {
    char trace[13] = "";
    int tids[8];
    args_t args[8] = {
        {trace, "1", -1, " ", 0, tids}, // 0
        {trace, "4",  4, "7", 1, tids}, // 1 <- 3
        {trace, "9", -1, " ", 2, tids}, // 2
        {trace, "2",  1, "8", 0, tids}, // 3
        {trace, "5", -1, " ", 1, tids}, // 4 <- 1
        {trace, "a", -1, " ", 2, tids}, // 5 <- 7
        {trace, "3", -1, " ", 0, tids}, // 6
        {trace, "6",  5, "b", 1, tids}  // 7
    };

    start_test("termino de threads");

    csetprio(0, 0); // Evita preempcao por prioridade

    for (int i = 0; i < 8; i++) {
      tids[i] = ccreate(func, &args[i], args[i].prio);
    }

    csetprio(0, 2); // Deve causar uma preempcao por prioridade

    strcat(trace, "c");

    assert("Quando uma thread termina, a thread que a espera (se houver) deve "
           "ser desbloqueada e ir para Aptos, e uma nova thread deve ser "
           "escalonada a partir da fila de Aptos, usando uma politica FIFO "
           "por prioridade",
           strcmp(trace, "123456789abc") == 0);

    int todas_terminaram = 1;
    for (int i = 0; i < 8; i++) {
      if (cjoin(tids[i]) >= 0) {
        todas_terminaram = 0;
        break;
      }
    }

    assert("A thread que terminou nao deve mais existir", todas_terminaram);

    return 0;
}
