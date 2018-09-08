#include "../include/cthread.h"
#include "teste.h"

#define MAX_NAME_SIZE 100

int main() {
    char name[MAX_NAME_SIZE];
    int ret_code;

    start_test("cidentify");

    for (int i = 0; i < MAX_NAME_SIZE; i++) {
      name[i] = 'A';
    }

    ret_code = cidentify(name, 2);

    assert("Retorna 0", ret_code == 0);

    int sobrescreveu = 0;
    for (int i = 2; i < MAX_NAME_SIZE; i++) {
      if (name[i] != 'A') {
        sobrescreveu = 1;
        break;
      }
    }

    assert("Respeita o tamanho maximo definido por size", !sobrescreveu);
}
