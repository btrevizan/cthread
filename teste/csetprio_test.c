#include "../include/cthread.h"
#include "teste.h"
#include <string.h>

void* func(void* trace) {
    strcat((char*) trace, "2");

    return NULL;
}

int main() {
    int ret_code;
    char trace[5] = "";

    start_test("csetprio");

    ret_code = csetprio(0, -1);
    assert("Se a prioridade for negativa retorna um codigo de erro.", 
           ret_code < 0);
    
    ret_code = csetprio(0, 3);
    assert("Se o valor de prioridade for maior que 2 retorna um codigo de "
           "erro.", ret_code < 0);



    ret_code = csetprio(0, 0);

    assert("Se executada corretamente, retorna 0", ret_code == 0);

    ccreate(func, trace, 0);

    strcat(trace, "1");

    csetprio(0, 1);

    strcat(trace, "3");

    assert("Se houver uma thread em Aptos com prioridade maior que a setada, "
           "deve ocorrer preempcao", strcmp(trace, "123") == 0);
}
