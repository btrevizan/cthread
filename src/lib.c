#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/helpers.h"

#define STACK_SIZE 16384

static states_t *states;
static PFILA2 threads;

static int first_run = 1;
static int tid = 0;

int ccreate (void* (*start)(void*), void *arg, int prio) {
	if (cthread_init() < 0) return -1;

	if (prio < 0 || prio > 2) return -1;

	TCB_t *thread = (TCB_t *) malloc(sizeof(TCB_t));

	thread->tid = get_tid();
	thread->state = PROCST_APTO;
	thread->prio = prio;
	thread->data = NULL;

	// Create the termination context
	ucontext_t *terminate_ctx = malloc(sizeof(ucontext_t));

	getcontext(terminate_ctx);

	terminate_ctx->uc_stack.ss_sp = malloc(STACK_SIZE);
	terminate_ctx->uc_stack.ss_size = STACK_SIZE;

	makecontext(terminate_ctx, (void (*)(void)) terminate, 1, thread);

	// Create the thread context
	getcontext(&thread->context);

	thread->context.uc_stack.ss_sp = malloc(STACK_SIZE);
	thread->context.uc_stack.ss_size = STACK_SIZE;
	thread->context.uc_link = terminate_ctx;

	makecontext(&thread->context, (void (*)(void)) start, 1, arg);

	// Add to thread list
	AppendFila2(threads, thread);

	set_ready(thread);

	return thread->tid;
}

int csetprio(int tid, int prio) {
	if (cthread_init() < 0) return -1;

	if (prio < 0 || prio > 2) return -1;

	states->running->prio = prio;

	return preemption();
}

int cyield(void) {
	if (cthread_init() < 0) return -1;
	
	return set_ready(states->running);
}

int cjoin(int tid) {
	if (cthread_init() < 0) return -1;
	
	TCB_t *tcb_blocked, *tcb_blocker; 

	tcb_blocked = (TCB_t *) states->running;
	tcb_blocker = (TCB_t *) search_TCB(tid);

	if(tcb_blocker == NULL) return -1;
	if(tcb_blocker->data != NULL) return -1;

	tcb_blocker->data = (TCB_t *) tcb_blocked;

	set_blocked(tcb_blocked);

	return 0;
}

int csem_init(csem_t *sem, int count) {
	if (cthread_init() < 0) return -1;

	sem->count = count;
	sem->fila = (PFILA2) malloc(sizeof(FILA2));
	
	if(CreateFila2(sem->fila))
		return -1;

	return 0;
}

int cwait(csem_t *sem) {
	if (cthread_init() < 0) return -1;
	return -1;
}

int csignal(csem_t *sem) {
	if (cthread_init() < 0) return -1;
	return -1;
}

int cidentify (char *name, int size) {
	if (cthread_init() < 0) return -1;
	strncpy (name, "Bernardo Trevizan - 00285638\nEduarda Trindade - 00274709\nGabriel Haggstrom - 00228552", size);
	return 0;
}

int get_tid(){
	return tid++;
}

int cthread_init() {
	if(first_run == 0)
		return 0;

	// Remove first run
	first_run--;

	// Initialize base structures
	states = (states_t *) malloc(sizeof(states_t));

	threads = (PFILA2) malloc(sizeof(FILA2));

	states->ready_high = (PFILA2) malloc(sizeof(FILA2));
	states->ready_medium = (PFILA2) malloc(sizeof(FILA2));
	states->ready_low = (PFILA2) malloc(sizeof(FILA2));

	if(CreateFila2(states->ready_high))
		return -1;

	if(CreateFila2(states->ready_medium))
		return -1;

	if(CreateFila2(states->ready_low))
		return -1;

	// Set main's TCB
	TCB_t *main_thread = (TCB_t *) malloc(sizeof(TCB_t));

	main_thread->tid = get_tid();  // zero
	main_thread->state = PROCST_EXEC;
	main_thread->prio = LOW_PRIO;
	main_thread->data = NULL;

	// Add to thread list
	AppendFila2(threads, main_thread);

	// Set on running
	states->running = main_thread;
	return 0;
}

TCB_t* scheduler() {
	TCB_t *next;

	if(FirstFila2(states->ready_high) == 0) {
		next = (TCB_t *) GetAtIteratorFila2(states->ready_high);
	} else if(FirstFila2(states->ready_medium) == 0) {
		next = (TCB_t *) GetAtIteratorFila2(states->ready_medium);
	} else if(FirstFila2(states->ready_low) == 0) {
		next = (TCB_t *) GetAtIteratorFila2(states->ready_low);
	} else {
		return NULL;
	}

	return next;
}

int dispatcher(TCB_t *old) {
	TCB_t *new = scheduler();
	if (new == NULL) 
		cthread_exit();
	
	change_context(old, new);

	return 0;
}

int change_context(TCB_t *old, TCB_t *new) {
	set_running(new);

	if(old == NULL)
		setcontext(&(new->context));
	else
		swapcontext(&(old->context), &(new->context));

	return 0;
}

int preemption() {
	TCB_t *new = scheduler();
	if(new != NULL && new->prio < states->running->prio) {
		// New thread has a higher priority
		// Remove the running thread and set it to ready
		set_ready_as_prio(states->running);

		// Change context between threads
		return change_context(states->running, new);
	}

	return 0;
}

int set_running(TCB_t *thread) {
	states->running = thread;

	switch(thread->prio) {
		case HIGH_PRIO:
			FirstFila2(states->ready_high);
			DeleteAtIteratorFila2(states->ready_high);
			break;

		case MEDIUM_PRIO:
			FirstFila2(states->ready_medium);
			DeleteAtIteratorFila2(states->ready_medium);
			break;

		case LOW_PRIO:
			FirstFila2(states->ready_low);
			DeleteAtIteratorFila2(states->ready_low);
	}

	return 0;
}

int set_ready(TCB_t *thread) {
	if(thread->tid == states->running->tid) {
		// Thread is executing
		states->running = NULL;
		set_ready_as_prio(thread);
		return dispatcher(thread);
	}

	set_ready_as_prio(thread);
	return preemption();
}

int set_ready_as_prio(TCB_t *thread) {
	int result;
	thread->state = PROCST_APTO;

	switch(thread->prio) {
		case HIGH_PRIO:
			result = AppendFila2(states->ready_high, thread);
			break;

		case MEDIUM_PRIO:
			result = AppendFila2(states->ready_medium, thread);
			break;

		default:
			result = AppendFila2(states->ready_low, thread);
	}

	return result;
}

int set_blocked(TCB_t *thread) {
	states->running = NULL;
	thread->state = PROCST_BLOQ;
	return dispatcher(thread);
}

TCB_t* search_TCB(int tid) {
    TCB_t *thread;
    int result = FirstFila2(threads);

    while(result == 0){
        thread = (TCB_t *) GetAtIteratorFila2(threads);

        if(thread->tid == tid)
            return thread;

        if(thread->data != NULL) {
            TCB_t *aux = (TCB_t *) thread->data;
            if (aux->tid == tid)
                return aux;
        }

        result = NextFila2(threads);
    }

    return NULL;
}

void terminate(TCB_t *thread) {
	//	Unblock the waiting thread, if exists
	if(thread->data != NULL)
		set_ready((TCB_t *) thread->data);

	// Remove from thread list
	TCB_t *aux;
	int it = FirstFila2(threads);

	while(it == 0){
			aux = (TCB_t *) GetAtIteratorFila2(threads);

			if(thread->tid == aux->tid) {
					DeleteAtIteratorFila2(threads);
					it = 1;
			} else {
					it = NextFila2(threads);
			}
	}

	// Remove from running
	states->running = NULL;

	free(thread);
	dispatcher(NULL);
}

int cthread_exit() {
	free(states);

	exit(0);
}