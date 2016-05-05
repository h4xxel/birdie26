/*
Copyright (c) 2015 Steven Arnow <s@rdw.se>
'platform_generic.h' - This file is part of libdangit

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

#include <stdint.h>
#include <stdbool.h>

#ifndef __PLATFORM_GENERIC_H__
#define	__PLATFORM_GENERIC_H__


typedef void DI_CC_MUTEX;
typedef void DI_CC_SEMAPHORE;
typedef void DI_CC_THREAD;

DI_CC_MUTEX *di_cc_mutex_new();
DI_CC_MUTEX *di_cc_mutex_destroy(DI_CC_MUTEX *);
bool di_cc_mutex_lock(DI_CC_MUTEX *);
bool di_cc_mutex_unlock(DI_CC_MUTEX *);
bool di_cc_mutex_trylock(DI_CC_MUTEX *);


DI_CC_SEMAPHORE *di_cc_semaphore_new(int);
DI_CC_SEMAPHORE *di_cc_semaphore_destroy(DI_CC_SEMAPHORE *);
bool di_cc_semaphore_wait(DI_CC_SEMAPHORE *);
bool di_cc_semaphore_trywait(DI_CC_SEMAPHORE *);
bool di_cc_semaphore_post(DI_CC_SEMAPHORE *);

DI_CC_THREAD *di_cc_thread_new(void *(*)(void *), void *);
void di_cc_thread_exit(void *);
bool di_cc_thread_join(DI_CC_THREAD *, void **);

#endif
