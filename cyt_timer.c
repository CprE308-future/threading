
#include <stdlib.h>

#define CLOCKID		CLOCK_REALTIME
#define SIGNAL		SIGALRM

struct cyt_timer
{
	timer_t timerid;
	
};

static void cyt_timer_OnTick(int sig, siginfo_t * siginfo, void* context)
{
	
}

int cyt_timer_Init()
{
	// create alternate signal stack for the timer
	stack_t sigstack;
	sigstack.ss_sp = malloc(SIGSTKSZ);
	if (sigstack.sp == NULL) {
		perror("malloc");
		return -1;
	}
	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	rv = sigaltstack(&sigstack, NULL);
	if ( rv < 0) {
		perror("sigaltstack");
		return -1;
	}
}

struct cyt_timer* cyt_timer_Create(long long freq_nanosecs)
{
	struct sigevent sig_event;
	struct itimerspec timer_spec;
	sigset_t sig_mask;
	struct sigaction sig_action;
	
	// create the signal handler for the timer
	sig_action.sa_flags = SA_SIGINFO 			// call sa_sigaction instead of sa_handler
						| SA_NODEFER 			// allow recursive signals
						| SA_ONSTACK;			// call the sigaction on an alternate stack
	sig_action.sa_sigaction = cyt_timer_OnTick;	// assign cyt_timer_OnTick() to be called as the handler
	sigemptyset(&sig_action.sa_mask);			// create an empty set of signals this handler catches
	rv = sigaction(SIGNAL, &sig_action, NULL);	// create the signal action handler
	if (rv < 0) {
		perror("sigaction");
		return -1;
	}
	
	// block the timer signal until we are ready for it
	sigemptyset(&sig_mask);
	sigaddset(&mask, SIGNAL);
	rv = sigprocmask(SIG_SETMASK, &sig_mask, NULL);
	if (rv < 0) {
		perror("sigprocmask");
		return -1;
	}
	
	// create the timer
	sig_event.sigev_notify = SIGEV_SIGNAL;		// raise a signal on the timer event
	sig_event.sigev_signo = SIGNAL;				// the signal to raise
	sig_event.sigev_value.sival_ptr = NULL;		// data that can be passed to the handler
	rv = timer_create(CLOCKID, &sig_event, &timer.timerid);	// create the timer
	if (rv < 0) {
		perror("timer_create");
		return -1;
	}
	
	
	
	
}


