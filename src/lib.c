#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/helpers.h"

static states_t *states;
static int first_run = 1;
static int tid = 0;

int ccreate (void* (*start)(void*), void *arg, int prio) {
	if (cthread_init() < 0) return -1;
	return -1;
}

int csetprio(int tid, int prio) {
	if (cthread_init() < 0) return -1;
	return -1;
}

int cyield(void) {
	if (cthread_init() < 0) return -1;
	return -1;
}

int cjoin(int tid) {
	if (cthread_init() < 0) return -1;
	return -1;
}

int csem_init(csem_t *sem, int count) {
	if (cthread_init() < 0) return -1;
	return -1;
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
		return -1;

	// Remove first run
	first_run--;

	// Initialize base structures
	states = (states_t *) malloc(sizeof(states_t));

	states->running = (TCB_t *) malloc(sizeof(TCB_t));

	if(CreateFila2(states->ready_high))
		return -1;

	if(CreateFila2(states->ready_medium))
		return -1;

	if(CreateFila2(states->ready_low))
		return -1;

	// Create main context
	ucontext_t *main_context = (ucontext_t *) malloc(sizeof(ucontext_t));

	if(getcontext(main_context) == -1)
		return -1;

	// Set main's TCB
	TCB_t *main_thread = (TCB_t *) malloc(sizeof(TCB_t));

	main_thread->tid = get_tid(); // zero
	main_thread->state = PROCST_EXEC;
	main_thread->prio = LOW_PRIO;
	main_thread->context = *main_context;

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
		cthread_exit();
		return NULL;
	}

	return next;
}

int dispatcher(TCB_t *old) {
	TCB_t *new = scheduler();
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
	if(new->prio > states->running->prio) {
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
	if(states->running != NULL)
		if(tid == states->running->tid)
			return states->running;

	TCB_t *result;

	result = search_TCB_in_queue(tid, states->ready_high);
	if(result != NULL)
		return result;

	result = search_TCB_in_queue(tid, states->ready_medium);
	if(result != NULL)
		return result;

	result = search_TCB_in_queue(tid, states->ready_low);
	if(result != NULL)
		return result;

	return NULL;
}

TCB_t* search_TCB_in_queue(int tid, PFILA2 queue){
	TCB_t *thread;
	int result = FirstFila2(queue);

	while(result == 0){
		thread = (TCB_t *) GetAtIteratorFila2(queue);

		if(thread->tid == tid)
			return thread;

		if(thread->data != NULL) {
			TCB_t *aux = (TCB_t *) thread->data;
			if (aux->tid == tid)
				return aux;
		}

		result = NextFila2(queue);
	}

	return NULL;
}

void terminate(TCB_t *thread) {
	//	Unblock the waiting thread, if exists
	if(thread->data != NULL)
		set_ready((TCB_t *) thread->data);

	states->running = NULL;
	free(thread);

	dispatcher(NULL);
}

int cthread_exit() {
	free(states);

	return 0;
}