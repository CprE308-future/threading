
#include "cyt_types.h"

// add a newly arrived task to the ready queue
void cyt_scheduler_OnArrive(struct cyt_thread * t);
// The given task has finished and no longer should be in the reaady or run queues
void cyt_scheduler_OnFinish(struct cyt_thread * t) __attribute__((noreturn));
void cyt_scheduler_OnSuspend(struct cyt_thread* t);
void cyt_scheduler_OnResume(struct cyt_thread* t);

struct scheduler
{
	// called when a thread first arrives
	struct cyt_thread* (*OnArrive)(struct cyt_thread* t);
	// called when a thread is finshed and will never run again
	struct cyt_thread* (*OnFinish)(struct cyt_thread* t);
	// called when a thread is nolonger able to be run, but may run again in the future
	struct cyt_thread* (*OnSuspend)(struct cyt_thread* t);
	// called when a thread that was previously suspended is ready to run again
	struct cyt_thread* (*OnResume)(struct cyt_thread* t);
	// called periodically
	struct cyt_thread* (*OnTick)(struct cyt_thread* running, unsigned long ticks);
};


void cyt_scheduler_SetScheduler(struct scheduler * sched);






