#include "../include/cthread.h"
#include "teste.h"
#include <string.h>

void* func_2(void* trace) {
    strcat((char*) trace, "2");
    cyield();

    return NULL;
}

void* func_3(void* trace) {
    strcat((char*) trace, "3");
    cyield();

    return NULL;
}

void* func_4(void* trace) {
    strcat((char*) trace, "4");
    cyield();

    return NULL;
}

void* high_prio(void* trace) {
    strcat((char*) trace, "1");
    cyield();
    strcat((char*) trace, "2");

    return NULL;
}

int main() {
    int ret_code;
    char trace[5] = "";

    start_test("cyield");

    ccreate(func_2, trace, 2);
    ccreate(func_3, trace, 2);
    ccreate(func_4, trace, 2);

    strcat(trace, "1");

    ret_code = cyield();

    assert("Retorna 0", ret_code == 0);

    assert("A proxima thread da fila eh executada, usando uma politica FIFO", 
           strcmp(trace, "1234") == 0);



    trace[0] = '\0'; // String vazia

    ccreate(high_prio, trace, 1);

    strcat(trace, "3");

    assert("Se a thread atual eh a de maior prioridade, e nao ha outra com "
           "mesma prioridade, deve continuar executando", 
           strcmp(trace, "123") == 0);
}
