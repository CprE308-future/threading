#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "cyt_scheduler.h"
#include "cyt_thread.h"
#include "cyt_context.h"
#include "cyt_timer.h"

#define CYT_PRIVATE
#include "cyt_thread_priv.h"

#define DEBUG
#ifdef DEBUG
#define dprintf(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define dprintf(fmt, ...)
#endif

extern struct cyt_thread main_thread;
extern struct cyt_thread idle_thread;

static struct cyt_thread *running_thread = &main_thread;

static struct scheduler const * cur_scheduler;

static struct cyt_timer * tick_timer;

void cyt_scheduler_OnTick(jiffy_t sys_time, void * context)
{
	printf(" %llu \n", sys_time);
	//sys_time ++;
	struct cyt_thread * to_run = cur_scheduler->OnTick(running_thread, sys_time);
	to_run = to_run ? to_run : &idle_thread;
	if(running_thread != to_run) {
		dprintf("Switch from thread %s to %s\n", cyt_thread_GetName(running_thread), cyt_thread_GetName(to_run));
		// save the context to the running thread
		cyt_context_CopyContext(running_thread->context, context);
		// save the newly selected running task
		running_thread = to_run;
		cyt_context_ResumeContext(running_thread->context);
	}
	// this will return back to @param context
	return;
}

void cyt_scheduler_Init(struct scheduler const * scheduler)
{
	cur_scheduler = scheduler;
	
	// create the context switch timer and start it
	tick_timer = cyt_timer_Create(0, 1, cyt_scheduler_OnTick, NULL);
	cyt_timer_Start(tick_timer);
}



void cyt_scheduler_OnArrive(struct cyt_thread* t)
{
	struct cyt_thread* to_run = cur_scheduler->OnArrive(t);
	to_run = to_run ? to_run : &idle_thread;
	if(to_run != running_thread)
	{
		// copy the old running thread so we can save its context
		struct cyt_thread* old = running_thread;
		// update running thread to reflect the newly chosen thread
		running_thread = to_run;
		// save the current context and switch to the new context
		cyt_context_SwitchContext(old->context, running_thread->context);
	}
}

void cyt_scheduler_OnFinish(struct cyt_thread* t) 
{
	struct cyt_thread* to_run = cur_scheduler->OnFinish(t);
	to_run = to_run ? to_run : &idle_thread;
	running_thread = to_run;
	cyt_context_ResumeContext(running_thread->context);
	dprintf("This is not good...");
	while(1); // trap
}

void cyt_scheduler_OnSuspend(struct cyt_thread* t)
{
	if(t == NULL)
		t = running_thread;
	struct cyt_thread* to_run = cur_scheduler->OnSuspend(t);
	to_run = to_run ? to_run : &idle_thread;
	if(to_run != running_thread)
	{
		// copy the old running thread so we can save its context
		struct cyt_thread* old = running_thread;
		// update running thread to reflect the newly chosen thread
		running_thread = to_run;
		// save the current context and switch to the new context
		cyt_context_SwitchContext(old->context, running_thread->context);
	}
}

void cyt_scheduler_OnResume(struct cyt_thread* t)
{
	struct cyt_thread* to_run = cur_scheduler->OnResume(t);
	to_run = to_run ? to_run : &idle_thread;
	if(to_run != running_thread)
	{
		// copy the old running thread so we can save its context
		struct cyt_thread* old = running_thread;
		// update running thread to reflect the newly chosen thread
		running_thread = to_run;
		// save the current context and switch to the new context
		cyt_context_SwitchContext(old->context, running_thread->context);
	}
}


