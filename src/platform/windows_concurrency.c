/*
Copyright (c) 2015 Steven Arnow <s@rdw.se>
'windows_concurrency.c' - This file is part of libdangit

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

#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

struct WindowsConcurrencyMutex {
	HANDLE				mutex;
};

struct WindowsConcurrencySemaphore {
	HANDLE				sem;
};

struct WindowsConcurrencyThread {
	HANDLE				thread;
	void				*arg;
	void				*(*func)(void *);
	void				*return_value;
};


/* Hack! To allow for threads to return a pointer, we need to know in the thread where the structure describing us lives... */
static __thread struct WindowsConcurrencyThread *this_thread;

struct WindowsConcurrencyMutex *di_cc_mutex_new() {
	struct WindowsConcurrencyMutex *cc_mutex;

	cc_mutex = malloc(sizeof(*cc_mutex));
	if (!(cc_mutex->mutex = CreateMutex(NULL, FALSE, NULL)))
		return free(cc_mutex), NULL;
	return cc_mutex;
}


struct WindowsConcurrencyMutex *di_cc_mutex_destroy(struct WindowsConcurrencyMutex *cc_mutex) {
	if (cc_mutex)
		CloseHandle(cc_mutex->mutex), free(cc_mutex);
	return NULL;
}


bool di_cc_mutex_lock(struct WindowsConcurrencyMutex *cc_mutex) {
	return cc_mutex?WaitForSingleObject(cc_mutex->mutex, INFINITE)==WAIT_OBJECT_0:false;
}


bool di_cc_mutex_unlock(struct WindowsConcurrencyMutex *cc_mutex) {
	return cc_mutex?!!ReleaseMutex(cc_mutex->mutex):false;
}


bool di_cc_mutex_trylock(struct WindowsConcurrencyMutex *cc_mutex) {
	return cc_mutex?WaitForSingleObject(cc_mutex->mutex, 0) == WAIT_OBJECT_0:false;
}


struct WindowsConcurrencySemaphore *di_cc_semaphore_new(int initial_value) {
	struct WindowsConcurrencySemaphore *sem;

	sem = malloc(sizeof(*sem));
	if (!(sem->sem = CreateSemaphore(NULL, initial_value, INT_MAX, NULL)))
		return free(sem), NULL;
	return sem;
}


struct WindowsConcurrencySemaphore *di_cc_semaphore_destroy(struct WindowsConcurrencySemaphore *sem) {
	if (sem)
		CloseHandle(sem->sem), free(sem);
	return NULL;
}


bool di_cc_semaphore_wait(struct WindowsConcurrencySemaphore *sem) {
	return sem?WaitForSingleObject(sem->sem, INFINITE)==WAIT_OBJECT_0:false;
}


bool di_cc_semaphore_trywait(struct WindowsConcurrencySemaphore *sem) {
	return sem?WaitForSingleObject(sem->sem, 0)==WAIT_OBJECT_0:false;
}


bool di_cc_semaphore_post(struct WindowsConcurrencySemaphore *sem) {
	return sem?!!ReleaseSemaphore(sem->sem, 1, NULL):false;
}


static DWORD entry_wrapper(void *data) {
	struct WindowsConcurrencyThread *thr = data;

	this_thread = thr;
	thr->return_value = thr->func(thr->arg);
	
	return 0;
}


struct WindowsConcurrencyThread *di_cc_thread_new(void *(*entry)(void *), void *data) {
	struct WindowsConcurrencyThread *thr;

	thr = malloc(sizeof(*thr));
	thr->func = entry;
	thr->arg = data;
	thr->return_value = NULL;
	if (!(thr->thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) entry_wrapper, thr, 0, NULL)))
		return free(thr), NULL;
	return thr;
}


void di_cc_thread_exit(void *exit_value) {
	this_thread->return_value = exit_value;
	ExitThread(0);
	return;
}


bool di_cc_thread_join(struct WindowsConcurrencyThread *thr, void **return_value) {
	bool success;
	if (!thr)
		return false;
	success = WaitForSingleObject(thr->thread, 0) == WAIT_OBJECT_0;
	if (return_value)
		*return_value = thr->return_value;
	free(thr);
	return success;
}
