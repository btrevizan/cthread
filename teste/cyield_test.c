#include "../include/cthread.h"
#include "teste.h"
#include <string.h>
#include <stdlib.h>

typedef struct args_t {
    char* trace;
    char number_str[2];
    int prio;
} args_t;

void* func(void* arg) {
    args_t* args = (args_t *) arg;

    strcat(args->trace, args->number_str);
    cyield();

    return NULL;
}

void* func2(void* trace) {
    strcat((char*) trace, "3");

    return NULL;
}

int main() {
    int ret_code;
    char trace[10] = "";
    args_t arguments[8] = {
        {trace, "4", 1},
        {trace, "7", 2},
        {trace, "2", 0},
        {trace, "5", 1},
        {trace, "8", 2},
        {trace, "3", 0},
        {trace, "6", 1},
        {trace, "9", 2}
    };
    int tid[8];
    int tid_func_2;

    start_test("cyield");

    csetprio(0, 0); // Evita preempção por prioridade

    strcat(trace, "1");

    for (int i = 0; i < 8; i++) {
        tid[i] = ccreate(func, &arguments[i], arguments[i].prio);
    }

    // Cada thread chama cyield para dar lugar à próxima thread
    ret_code = cyield();

    assert("Retorna 0", ret_code == 0);

    // Testa se a ordem de execução foi a correta
    assert("A proxima thread da fila eh executada, usando uma politica FIFO "
           "por prioridade", strcmp(trace, "123456789") == 0);

    for (int i = 0; i < 8; i++) {
        cjoin(tid[i]);
    }


    trace[0] = '\0'; // String vazia

    tid_func_2 = ccreate(func2, trace, 1);

    strcat(trace, "1");

    cyield();

    strcat(trace, "2");

    assert("Se a thread atual eh a de maior prioridade, e nao ha outra com "
           "mesma prioridade, deve continuar executando", 
           strcmp(trace, "12") == 0);

    cjoin(tid_func_2);

    end_test();
    return 0;
}
