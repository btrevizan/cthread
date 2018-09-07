#include "../include/cthread.h"
#include "teste.h"

void* func(void* arg) {
    *((int*) arg) = 1;

    return NULL;
}

int main() {
    int tid;
    int arg;

    start_test("ccreate");

    tid = ccreate(func, &arg, -1);
    assert("Se a prioridade for negativa retorna um codigo de erro.", tid < 0);
    
    tid = ccreate(func, &arg, 3);
    assert("Se o valor de prioridade for maior que 2 retorna um codigo de "
           "erro.", tid < 0);



    arg = 0;

    tid = ccreate(func, &arg, 2);

    assert("O tid deve ser positivo", tid > 0);

    assert("Nao deve ocorrer preempcao se a prioridade da thread criada for "
           "igual a atual.", arg == 0);

    cjoin(tid);

    assert("A funcao da thread recebe o ponteiro para o argumento.", arg == 1);



    arg = 0;

    csetprio(0, 1);

    tid = ccreate(func, &arg, 2);

    assert("Nao deve ocorrer preempcao se a prioridade da thread criada for "
           "menor que a atual.", arg == 0);

    cjoin(tid);



    arg = 0;

    tid = ccreate(func, &arg, 0);

    assert("Deve ocorrer preempcao se a prioridade da thread criada for "
           "maior que a atual.", arg == 1);

    cjoin(tid);
}
