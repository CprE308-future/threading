
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ucontext.h>
#include <stdatomic.h>

#include "cyt_thread.h"
#include "cyt_scheduler.h"
#include "cyt_context.h"

#define CYT_PRIVATE
#include "cyt_thread_priv.h"

#define GB(x) KB(MB(x))
#define MB(x) KB(KB(x))
#define KB(x) (x*1024)

#ifdef DEBUG
#define dprintf(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define dprintf(fmt, ...)
#endif

static void cyt_thread_OnStart(struct cyt_thread * thread);

struct cyt_thread main_thread;
struct cyt_thread idle_thread;

void cyt_thread_IdleLoop(void* param)
{
	volatile long i;
	while(1) {
		printf(".");fflush(stdout);
		for(i=0;i<1000000000;i++); // busy wait	
		//cyt_thread_Resume(&main_thread);
	}
}

// start the thread by sending it to the scheduler
static void cyt_thread_Start(struct cyt_thread* t)
{
	cyt_scheduler_OnArrive(t);
}

void cyt_thread_Init()
{
	idle_thread.id = 0;
	idle_thread.function = NULL;
	idle_thread.info.name = "IDLE";
	idle_thread.context = cyt_context_Create(MB(1), cyt_thread_IdleLoop, NULL);
	
	main_thread.id = 1;
	main_thread.function = NULL;
	main_thread.info.name = "MAIN";
	main_thread.context = cyt_context_CreateMain();
	
	cyt_scheduler_OnArrive(&main_thread); // should return imediatly because this *should* be the only thread in the system.
}

struct cyt_thread* cyt_thread_Create(thread_func f, void * param, const char * name)
{
	static atomic_int id = 2;
	struct cyt_thread * thread = calloc(1, sizeof(struct cyt_thread));
	thread->id = atomic_fetch_add(&id, 1);
	thread->function = f;
	thread->param = param;
	thread->info.name = malloc(strlen(name)+1);
	strcpy(thread->info.name, name);
	
	thread->context = cyt_context_Create(MB(1), (context_on_start_t)cyt_thread_OnStart, thread);
	
	cyt_thread_Start(thread);
	
	return thread;
}

void cyt_thread_Delete(struct cyt_thread * thread)
{
	cyt_context_Delete(thread->context);
	free(thread->info.name);
	free(thread);
}


void cyt_thread_Suspend(struct cyt_thread * t)
{
	dprintf("Suspending thread %s\n", t ? t->info.name : "");
	cyt_scheduler_OnSuspend(t);
}

void cyt_thread_Resume(struct cyt_thread * t)
{
	dprintf("Resuming thread %s\n", t->info.name);
	cyt_scheduler_OnResume(t);
}


static void cyt_thread_OnStart(struct cyt_thread * thread)
{
	dprintf("Starting thread %s\n", thread->info.name);
	thread->function(thread->param);
	dprintf("Thread %s finished\n", thread->info.name);
	cyt_scheduler_OnFinish(thread);
	cyt_thread_Delete(thread); /// @todo get rid of this and add in joining and detaching
}

char const * cyt_thread_GetName(struct cyt_thread* t) 
{
	return t->info.name;
}








