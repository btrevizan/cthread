#ifndef TESTE_H
#define TESTE_H

#include <stdio.h>

#define print_success(description) \
    printf("SUCCESS %s\n", description)

#define print_fail(description) \
    printf("FAIL %s\n", description)

void start_test(char* title) {
    printf("\n%s\n==========================================================\n",
        title);
}

void assert(char* description, int assertion) {
    if (assertion) {
        print_success(description);
    } else {
        print_fail(description);
    }
}

#endif //TESTE_H