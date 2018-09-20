// Funções auxiliares
#ifndef CTHREAD_HELPERS_H
#define CTHREAD_HELPERS_H

#include "support.h"
#include "cdata.h"

#define HIGH_PRIO 0
#define MEDIUM_PRIO 1
#define LOW_PRIO 2

typedef struct s_states{
    PFILA2 ready_high;		// fila de alta prioridade
    PFILA2 ready_medium;	// fila de média prioridade
    PFILA2 ready_low;		// fila de baixa prioridade
    TCB_t* running;
} states_t;

int get_tid();
int preemption();
int cthread_init();
int cthread_exit();
TCB_t* scheduler();
int init_first_run();
TCB_t* search_TCB(int tid);
TCB_t* search_TCB_in_queue(int tid, PFILA2 queue);
int dispatcher(TCB_t *old);
int set_ready(TCB_t *thread);
void terminate(TCB_t *thread);
int set_running(TCB_t *thread);
int set_blocked(TCB_t *thread);
int set_ready_as_prio(TCB_t *thread);
int change_context(TCB_t *old, TCB_t *new);

#endif //CTHREAD_HELPERS_H
