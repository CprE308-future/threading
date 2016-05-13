#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "cyt_scheduler.h"
#include "cyt_thread.h"
#include "cyt_context.h"

extern struct cyt_thread main_thread;

static struct cyt_thread *running_thread = &main_thread;



static struct scheduler const * cur_scheduler;


static void handler(int sig, siginfo_t * si, void * _context)
{
   timer_t *tidp;
   ucontext_t * context = _context;
   int or;

   tidp = si->si_value.sival_ptr;

   printf("    sival_ptr = %p; ", si->si_value.sival_ptr);
   printf("    *sival_ptr = 0x%lx\n", (long) *tidp);

   or = timer_getoverrun(*tidp);
   if (or == -1) {
       perror("timer_getoverrun");
   }
   else
       printf("    overrun count = %d\n", or);
   //signal(sig, SIG_IGN);
}

static unsigned long long sys_time = 0;

void cyt_scheduler_OnTick(int sig, siginfo_t * siginfo, void* context)
{
	sys_time ++;
	struct cyt_thread * to_run = cur_scheduler->OnTick(running_thread, sys_time);
	if(running_thread != to_run) {
		printf("Switch from thread %s to %s\n", running_thread->info.name, to_run->info.name);
		// save the context to the running thread
		cyt_context_CopyContext(running_thread->context, context);
		// save the newly selected running task
		running_thread = to_run;
		cyt_context_ResumeContext(running_thread->context);
	}
	// this will return back to @param context
	return;
}

static timer_t timerid;
void cyt_scheduler_Init(struct scheduler const * scheduler)
{
	cur_scheduler = scheduler;
	
	int rv;
	
	
	struct sigevent sev;
	struct itimerspec its;
	long long freq_nanosecs = 100000000ll;
	sigset_t mask;
	struct sigaction sa;
	
	#define CLOCKID CLOCK_REALTIME //CLOCK_PROCESS_CPUTIME_ID
	#define SIG SIGALRM
	
	// create handler for timer signal
	// todo, add option SA_ONSTACK and SA_NODEFER
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = cyt_scheduler_OnTick;
	sigemptyset(&sa.sa_mask);
	rv = sigaction(SIG, &sa, NULL);
	if (rv < 0) {
		perror("sigaction");
		return;
	}
	
	// block timer signal
	sigemptyset(&mask);
	sigaddset(&mask, SIG);
	rv = sigprocmask (SIG_SETMASK, &mask, NULL);
	if (rv < 0) {
		perror("sigprocmask");
		return;
	}
	
	// create timer
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;
	rv = timer_create(CLOCKID, &sev, &timerid);
	if (rv < 0) {
		perror("timer_create");
		return;
	}
	
	// start timer (probably move to another function)
	its.it_value.tv_sec = freq_nanosecs / 1000000000;
	its.it_value.tv_nsec = freq_nanosecs % 1000000000;
	its.it_interval.tv_sec = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;
	
	rv = timer_settime(timerid, 0, &its, NULL);
	if (rv < 0) {
		perror("timer_settime");
		return;
	}
	
	rv = sigprocmask(SIG_UNBLOCK, &mask, NULL);
	if (rv < 0) {
		perror("sigprocmask");
		return;
	}
}



void cyt_scheduler_OnArrive(struct cyt_thread* t)
{
	struct cyt_thread* to_run = cur_scheduler->OnArrive(t);
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
	running_thread = to_run;
	cyt_context_ResumeContext(running_thread->context);
	printf("This is not good...");
	while(1); // trap
}

void cyt_scheduler_OnSuspend(struct cyt_thread* t)
{
	if(t == NULL)
		t = running_thread;
	struct cyt_thread* to_run = cur_scheduler->OnSuspend(t);
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


