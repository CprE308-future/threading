



#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "cyt_systick.h"
#include "cyt_timer.h"

#include "cyt_timer_priv.h"

#define CLOCKID		CLOCK_REALTIME
#define SIGNAL		SIGALRM


static timer_t systick_id;
static jiffy_t system_time = 0;

static struct itimerspec timer_spec;
static sigset_t sig_mask;

//static struct cyt_timer * timer_list;
LLIST_CREATE_HEAD(struct cyt_timer, list, timer_list);

static void cyt_systick_OnSysTick(int sig, siginfo_t * siginfo, void* context)
{
	system_time ++;
	struct cyt_timer * i;
	
	//for(i=LLIST_NEXT(list, timer_list); i != timer_list; i=LLIST_NEXT(list, i)) 
	LLIST_FOREACH(list, timer_list, i){
		//printf("in foreach\n");
		if (i->next == system_time) {
			void * param;
			if (i->period) {
				i->next += i->period;
			} else {
				i->is_running = false;
				LLIST_REMOVE(list,i);
			}
			param = i->param ? i->param : context;
			i->callback(system_time, param);
		}
	}
	//printf("systick!\n");
}

int cyt_systick_Init(unsigned long hz)
{
	unsigned long long ns = 1000000000ll/hz;
	int rv;
	struct sigevent sig_event;
	struct sigaction sig_action;
	
	// create alternate signal stack (might move to cyt_Init())
	stack_t sigstack;
	sigstack.ss_sp = malloc(SIGSTKSZ);
	if (sigstack.ss_sp == NULL) {
		perror("malloc");
		return -1;
	}
	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	rv = sigaltstack(&sigstack, NULL);
	if (rv < 0) {
		perror("sigaltstack");
		free(sigstack.ss_sp);
	}
	
	// create the signal handler for the timer
	sig_action.sa_flags = SA_SIGINFO 			// call sa_sigaction instead of sa_handler
						//| SA_NODEFER 			// allow recursive signals
						;//| SA_ONSTACK;			// call the sigaction on an alternate stack
	sig_action.sa_sigaction = cyt_systick_OnSysTick;	// assign cyt_timer_OnTick() to be called as the handler
	sigemptyset(&sig_action.sa_mask);			// create an empty set of signals this handler catches
	rv = sigaction(SIGNAL, &sig_action, NULL);	// create the signal action handler
	if (rv < 0) {
		perror("sigaction");
		return -1;
	}
	
	// block the timer signal until we are ready for it
	sigemptyset(&sig_mask);
	sigaddset(&sig_mask, SIGNAL);
	rv = sigprocmask(SIG_SETMASK, &sig_mask, NULL);
	if (rv < 0) {
		perror("sigprocmask");
		return -1;
	}
	
	// create the timer
	sig_event.sigev_notify = SIGEV_SIGNAL;		// raise a signal on the timer event
	sig_event.sigev_signo = SIGNAL;				// the signal to raise
	sig_event.sigev_value.sival_ptr = NULL;		// data that can be passed to the handler
	rv = timer_create(CLOCKID, &sig_event, &systick_id);	// create the timer
	if (rv < 0) {
		perror("timer_create");
		return -1;
	}
	
	// start timer (probably move to another function)
	timer_spec.it_value.tv_sec = ns / 1000000000l;
	timer_spec.it_value.tv_nsec = ns % 1000000000l;
	timer_spec.it_interval.tv_sec = timer_spec.it_value.tv_sec;
	timer_spec.it_interval.tv_nsec = timer_spec.it_value.tv_nsec;
	
	// init the head of the linked list of timers
	LLIST_INIT(list, timer_list);
	
	return 0;
}

int cyt_systick_Start()
{
	int rv;
	rv = timer_settime(systick_id, 0, &timer_spec, NULL);
	if (rv < 0) {
		perror("timer_settime");
		return -1;
	}
	
	rv = sigprocmask(SIG_UNBLOCK, &sig_mask, NULL);
	if (rv < 0) {
		perror("sigprocmask");
		return -1;
	}
	return 0;
}

void cyt_systick_Pause()
{
	int rv = sigprocmask(SIG_BLOCK, &sig_mask, NULL);
	if (rv < 0) {
		perror("sigprocmask");
	}
}

void cyt_systick_Resume()
{
	int rv = sigprocmask(SIG_UNBLOCK, &sig_mask, NULL);
	if (rv < 0) {
		perror("sigprocmask");
	}
}


// register a new timer
void cyt_systick_RegisterTimer(struct cyt_timer * timer)
{
	printf("Registering timer\n");
	cyt_systick_Pause();
	if(timer->start) {
		timer->next = system_time + timer->start;
	} else if (timer->period) {
		timer->next = system_time + timer->period;
	} else {
		cyt_systick_Resume();
		return;
	}
	timer->is_running = true;
	LLIST_INSERT_BEFORE(list, timer_list, timer);
	
	cyt_systick_Resume();
}

// unregister a timer
void cyt_systick_UnregisterTimer(struct cyt_timer * timer)
{
	cyt_systick_Pause();
	if(timer->is_running){
		LLIST_REMOVE(list, timer);
	}
	cyt_systick_Resume();
}

