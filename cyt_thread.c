
#include <stdlib.h>
#include <stdio.h>

#include "cyt_thread.h"
#include "cyt_scheduler.h"
#include "cyt_context.h"


static void cyt_thread_OnStart(struct cyt_thread * thread);

struct cyt_thread main_thread;

// start the thread by sending it to the scheduler
static void cyt_thread_Start(struct cyt_thread* t)
{
	cyt_scheduler_OnArrive(t);
}

void cyt_thread_Init()
{
	main_thread.id = 1;
	main_thread.function = NULL;
	main_thread.info.name = "MAIN";
	main_thread.context = cyt_context_CreateMain();
	cyt_scheduler_OnArrive(&main_thread); // should return imediatly becaust this *should* be the only thread in the system.
}

struct cyt_thread* cyt_thread_Create(thread_func f, void * param, const char * name)
{
	static atomic_int id = 2;
	cyt_thread_t thread = calloc(1, sizeof(struct cyt_thread));
	thread->id = atomic_fetch_add(&id, 1);
	thread->function = f;
	thread->param = param;
	thread->info.name = malloc(strlen(name)+1);
	strcpy(thread->info.name, name);
	
	thread->context = cyt_context_Create(4096, cyt_thread_OnStart, thread);
	
	cyt_thread_Start(thread);
	
	return thread;
}


void cyt_thread_Suspend(cyt_thread_t t)
{
	printf("Suspending thread %s\n", t ? t->info.name : "");
	cyt_scheduler_OnSuspend(t);
}

void cyt_thread_Resume(struct cyt_thread * t)
{
	printf("Resuming thread %s\n", t->info.name);
	cyt_scheduler_OnResume(t);
}


static void cyt_thread_OnStart(struct cyt_thread * thread)
{
	printf("Starting thread %s\n", thread->info.name);
	thread->function(thread->param);
	printf("Thread %s finished\n", thread->info.name);
	cyt_scheduler_OnFinish(thread);
}










