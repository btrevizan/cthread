#include "../include/cthread.h"
#include "teste.h"
#include <string.h>

typedef struct args_t {
  char trace[5];
  csem_t semaforo;
} args_t;

void* func_1(void* arg) {
    args_t* args = (args_t*) arg;

    strcat(args->trace, "1");
    
    cwait(&args->semaforo);

    strcat(args->trace, "2");

    csignal(&args->semaforo);

    return NULL;
}

int main() {
    args_t args = {""};
    int ret_code;
    int tid1, tid2;

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
}
