#include "../include/cthread.h"
#include "teste.h"
#include <string.h>

typedef struct args_t {
  char trace[5];
  csem_t semaforo;
} args_t;

typedef struct args2_t {
    char* trace;
    char number_str[2];
    int prio;
    csem_t* semaforo;
} args2_t;

void* func_1(void* arg) {
    args_t* args = (args_t*) arg;

    strcat(args->trace, "1");
    
    cwait(&args->semaforo);

    strcat(args->trace, "2");

    csignal(&args->semaforo);

    return NULL;
}

void* func_2(void* arg) {
    args2_t* args = (args2_t*) arg;

    strcat(args->trace, args->number_str);

    if (args->semaforo->count == -8) {
        csignal(args->semaforo);

        return NULL;
    }
    
    cwait(args->semaforo);
    csignal(args->semaforo);

    return NULL;
}

int main() {
    args_t args = {""};
    int ret_code;
    int tid1, tid2;

    char trace2[10] = "";
    csem_t semaforo2;
    args2_t args2[8] = {
        {trace2, "4", 1, &semaforo2},
        {trace2, "7", 2, &semaforo2},
        {trace2, "2", 0, &semaforo2},
        {trace2, "5", 1, &semaforo2},
        {trace2, "8", 2, &semaforo2},
        {trace2, "3", 0, &semaforo2},
        {trace2, "6", 1, &semaforo2},
        {trace2, "9", 2, &semaforo2}
    };

    start_test("cwait");

    csem_init(&args.semaforo, 1);

    ret_code = cwait(&args.semaforo);

    assert("Se o contador eh maior que 0, continua executando", 1);

    assert("Retorna 0", ret_code == 0);

    assert("O contador eh decrementado em 1 unidade", args.semaforo.count == 0);



    tid1 = ccreate(func_1, &args, 2);
    cyield();
    
    assert("Se o contador eh igual a 0, a thread eh bloqueada", 
           strcmp(args.trace, "1") == 0);

    

    tid2 = ccreate(func_1, &args, 2);
    cyield();

    assert("Se o contador eh menor que 0, a thread eh bloqueada", 
           strcmp(args.trace, "11") == 0);
    
    // Libera os recursos e termina as threads de teste
    csignal(&args.semaforo);
    cjoin(tid1);
    cjoin(tid2);



    csem_init(&semaforo2, 0);

    csetprio(0, 0); // Evita preempção por prioridade

    strcat(trace2, "1");

    for (int i = 0; i < 8; i++) {
        ccreate(func_2, &args2[i], args2[i].prio);
    }

    // Cada thread chama cwait, sendo bloqueada.
    // A última thread chama csignal, desbloqueando as threads anteriores.
    cwait(&semaforo2);

    // Testa se a ordem de execução foi a correta
    assert("Quando uma thread eh bloqueada, a proxima thread da fila eh "
           "executada, usando uma politica FIFO por prioridade", 
           strcmp(trace2, "123456789") == 0);
}
