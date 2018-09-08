#include "../include/cthread.h"
#include "teste.h"
#include <string.h>

typedef struct args_t {
  int tid;
  char trace[5];
  int ret_code;
} args_t;

typedef struct args2_t {
    char* trace;
    char number_str[2];
    int prio;
    int next_tid;
} args2_t;

void* func_1(void* arg) {
    args_t* args = (args_t*) arg;

    strcat(args->trace, "1");
    args->ret_code = cjoin(args->tid);
    strcat(args->trace, "2");

    return NULL;
}

void* func_2(void* arg) {
    args_t* args = (args_t*) arg;

    strcat(args->trace, "3");

    return NULL;
}

void* func_3(void* arg) {
    args2_t* args = (args2_t*) arg;

    strcat(args->trace, args->number_str);

    if (args->next_tid == -1) {
        return NULL;
    }
    
    cjoin(args->next_tid);

    return NULL;
}

int main() {
    int ret_code;
    args_t args = {-1, "", 1};
    char trace2[10] = "";
    args2_t args2[9] = {
        {NULL,   "",  0},
        {trace2, "4", 1},
        {trace2, "7", 2},
        {trace2, "2", 0},
        {trace2, "5", 1},
        {trace2, "8", 2},
        {trace2, "3", 0},
        {trace2, "6", 1},
        {trace2, "9", 2}
    };

    start_test("cjoin");

    ret_code = cjoin(1);

    assert("Se a thread nao existe, retorna um codigo de erro", ret_code < 0);

    ccreate(func_1, &args, 2);
    args.tid = ccreate(func_2, &args, 2);

    ret_code = cjoin(args.tid);

    strcat(args.trace, "4");

    assert("A primeira chamada para um tid retorna 0", ret_code == 0);

    assert("As demais chamadas para um tid retornam um codigo de erro",
           args.ret_code < 0);

    assert("A primeira chamada para um tid eh bloqueada, e as demais "
           "retornam imediatamente", strcmp(args.trace, "1234") == 0);



    csetprio(0, 0); // Evita preempção por prioridade

    strcat(trace2, "1");

    for (int i = 1; i < 9; i++) {
        args2[i - 1].next_tid = ccreate(func_3, &args2[i], args2[i].prio);
    }
    args2[8].next_tid = -1;

    // Cada thread faz cjoin da thread seguinte, formando uma cadeia de cjoins.
    // A última thread não faz cjoin e termina, desbloqueando as threads 
    // anteriores.
    cjoin(args2[0].next_tid);

    // Testa se a ordem de execução foi a correta
    assert("Quando uma thread eh bloqueada, a proxima thread da fila eh "
           "executada, usando uma politica FIFO por prioridade", 
           strcmp(trace2, "123456789") == 0);

    return 0;
}
