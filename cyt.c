


#include "cyt_types.h"
#include "cyt_thread.h"
#include "cyt_scheduler.h"
#include "cyt_systick.h"

#define HZ(x)  (x)
#define KHZ(x) ((x)*1000)
#define MHZ(x) ((x)*1000000)
#define GHZ(x) ((x)*1000000000)

extern void cyt_thread_Init();
extern void cyt_scheduler_Init();

void cyt_Init(struct scheduler * sched)
{
	cyt_systick_Init(HZ(10));
	cyt_scheduler_Init(sched);
	cyt_thread_Init();
	
	cyt_systick_Start();
}
