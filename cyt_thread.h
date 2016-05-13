


#ifndef CYT_THREAD_H
#define CYT_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "cyt_types.h"

#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <stdatomic.h>

// make linked list macros
// use offsetof
struct llist
{
	struct llist* next;
	struct llist* prev;
};
typedef struct llist llist;


typedef struct cyt_thread * cyt_thread_t;
typedef void*(*thread_func)(void*);


struct cyt_thread
{
	llist queue;
	unsigned int id;
	thread_func function;
	void* param;
	struct cyt_mutex *mutex_list;
	struct cyt_sem *sem_list;
	struct cyt_context * context;
	struct cyt_thread_info
	{
		char * name;
		clock_t arrive_time;
	} info;
};


cyt_thread_t cyt_thread_Create(thread_func f, void * param, const char * name);
void cyt_thread_Suspend(cyt_thread_t t);
void cyt_Init();


#ifdef __cplusplus
}
#endif

#endif

