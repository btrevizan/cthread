#include "../include/cthread.h"
#include "teste.h"

int main() {
    csem_t semaforo = {0, NULL};
    int ret_code;

    start_test("csem_init");

    ret_code = csem_init(&semaforo, 3);

    assert("Retorna 0", ret_code == 0);

    assert("O contador deve iniciar com o valor fornecido",
           semaforo.count == 3);

    assert("A fila inicia vazia", 
           semaforo.fila && semaforo.fila->first == NULL);

    return 0;
}
