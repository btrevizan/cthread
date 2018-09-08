#include "../include/cthread.h"
#include "teste.h"
#include <string.h>

typedef struct args_t {
    char* trace;
    char number_str[2];
    int prio;
    int order;
    csem_t* semaforo;
    int* p_unblocker_tid;
} args_t;

void* unblocker(void* arg);

void* func_1(void* arg) {
    args_t* args = (args_t*) arg;

    // Espera sua vez para bloquear
    if (args->semaforo->count != -args->order) {
        cyield();
    }

    // Se for a última thread, cria uma thread que desbloqueia todas as outras
    if (args->order == 8) {
        *args->p_unblocker_tid = ccreate(unblocker, args->semaforo, 2);
    }

    csetprio(0, args->prio);
    cwait(args->semaforo);

    strcat(args->trace, args->number_str);

    return NULL;
}

void* unblocker(void* semaforo) {
    for (int i = 0; i < 9; i++) {
        csignal((csem_t*) semaforo);
        cyield();
    }

    return NULL;
}

void* func_2(void* arg) {
    args_t* args = (args_t*) arg;

    strcat(args->trace, "1");
    csignal(args->semaforo);
    strcat(args->trace, "3");

    return NULL;
}

int main() {
    csem_t semaforo;
    int ret_code;
    int old_count;

    char trace[10] = "";
    int tid, unblocker_tid;
    args_t args[8] = {
        {trace, "4", 1, 1, &semaforo, NULL},
        {trace, "7", 2, 2, &semaforo, NULL},
        {trace, "2", 0, 3, &semaforo, NULL},
        {trace, "5", 1, 4, &semaforo, NULL},
        {trace, "8", 2, 5, &semaforo, NULL},
        {trace, "3", 0, 6, &semaforo, NULL},
        {trace, "6", 1, 7, &semaforo, NULL},
        {trace, "9", 2, 8, &semaforo, &unblocker_tid}
    };

    start_test("csignal");

    csem_init(&semaforo, 1);

    old_count = semaforo.count;
    cwait(&semaforo);
    ret_code = csignal(&semaforo);

    assert("Retorna 0", ret_code == 0);

    assert("O contador eh incrementado em 1 unidade", 
           semaforo.count - old_count == 1);

    

    cwait(&semaforo); // semaforo.count == 0

    for (int i = 0; i < 8; i++) {
        ccreate(func_1, &args[i], 2);
    }

    // Cada thread chama cwait, sendo bloqueada.
    // A última thread chama csignal, desbloqueando as threads anteriores.
    csetprio(0, 0);
    cwait(&semaforo);

    strcat(trace, "1");

    cjoin(unblocker_tid);

    // Testa se a ordem de execução foi a correta
    assert("Quando uma thread eh desbloqueada, a proxima thread da fila do "
           "semaforo eh executada, usando uma politica FIFO por prioridade", 
           strcmp(trace, "123456789") == 0);



    tid = ccreate(func_2, &args[0], 2);
    cwait(&semaforo);
    strcat(trace, "2");
    cjoin(tid);

    assert("Quando uma thread com prioridade maior que a atual eh desbloqueada "
           "deve ocorrer preempcao por prioridade", strcmp(trace, "123") == 0);
}
