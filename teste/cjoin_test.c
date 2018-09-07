#include "../include/cthread.h"
#include "teste.h"
#include <string.h>

typedef struct args_t {
  int tid;
  char trace[5];
  int ret_code;
} args_t;

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

int main() {
    int ret_code;
    args_t args = {-1, "", 1};

    start_test("csetprio");

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
}
