/*
Copyright (c) 2015 Steven Arnow <s@rdw.se>
'posix_concurrency.c' - This file is part of libdangit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

//#include "posix_concurrency.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


struct PosixConcurrencyMutex {
	pthread_mutex_t		mutex;
};


struct PosixConcurrencySemaphore {
	sem_t			sem;
};


struct PosixConcurrencyThread {
	pthread_t		thr;
};


struct PosixConcurrencyMutex *di_cc_mutex_new() {
	struct PosixConcurrencyMutex *cc_mutex;

	cc_mutex = malloc(sizeof(*cc_mutex));
	pthread_mutex_init(&cc_mutex->mutex, NULL);

	return cc_mutex;
}


struct PosixConcurrencyMutex *di_cc_mutex_destroy(struct PosixConcurrencyMutex *cc_mutex) {
	if (cc_mutex)
		pthread_mutex_destroy(&cc_mutex->mutex), free(cc_mutex);

	return NULL;
}


bool di_cc_mutex_lock(struct PosixConcurrencyMutex *cc_mutex) {
	return cc_mutex?!pthread_mutex_lock(&cc_mutex->mutex):false;
}


bool di_cc_mutex_unlock(struct PosixConcurrencyMutex *cc_mutex) {
	return cc_mutex?!pthread_mutex_unlock(&cc_mutex->mutex):false;
}


bool di_cc_mutex_trylock(struct PosixConcurrencyMutex *cc_mutex) {
	return cc_mutex?!pthread_mutex_trylock(&cc_mutex->mutex):false;
}


struct PosixConcurrencySemaphore *di_cc_semaphore_new(int initial_value) {
	struct PosixConcurrencySemaphore *sem;

	sem = malloc(sizeof(*sem));
	sem_init(&sem->sem, 0, initial_value);

	return sem;
}

struct PosixConcurrencySemaphore *di_cc_semaphore_destroy(struct PosixConcurrencySemaphore *sem) {
	if (sem)
		sem_destroy(&sem->sem), free(sem);
	
	return NULL;
}


bool di_cc_semaphore_wait(struct PosixConcurrencySemaphore *sem) {
	return sem?!sem_wait(&sem->sem):false;
}


bool di_cc_semaphore_trywait(struct PosixConcurrencySemaphore *sem) {
	return sem?!sem_trywait(&sem->sem):false;
}


bool di_cc_semaphore_post(struct PosixConcurrencySemaphore *sem) {
	return sem?!sem_post(&sem->sem):false;
}

struct PosixConcurrencyThread *di_cc_thread_new(void *(*entry)(void *), void *data) {
	pthread_t thr;
	struct PosixConcurrencyThread *thread;
	thread = malloc(sizeof(*thread));

	if (!pthread_create(&thr, NULL, entry, data)) {
		thread->thr = thr;
		return thread;
	}

	return NULL;
}


void di_cc_thread_exit(void *exit_value) {
	pthread_exit(exit_value);
}


bool di_cc_thread_join(struct PosixConcurrencyThread *thr, void **return_value) {
	void *ptr;

	if (!thr)
		return false;
	if (!pthread_join(thr->thr, &ptr))
		return false;
	if (return_value)
		*return_value = ptr;
	free(thr);
	return true;
}
