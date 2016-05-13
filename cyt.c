


#include "cyt_types.h"
#include "cyt_thread.h"
#include "cyt_scheduler.h"

extern void cyt_thread_Init();
extern void cyt_scheduler_Init();

void cyt_Init(struct scheduler * sched)
{
	cyt_scheduler_Init(sched);
	cyt_thread_Init();
}
