
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "cyt_timer.h"
#include "cyt_systick.h"

#include "cyt_timer_priv.h"

//typedef void (timer_callback)(jiffy_t sys_time);


// start: the time in jffies to wait before the first timer call.  If zero the timer will first be called after period time
// period: the time interval in jiffies between callback calls.  If zero the timer is treated as a oneshot and will only be called once after start time
struct cyt_timer * cyt_timer_Create(jiffy_t start, jiffy_t period, timer_callback callback, void * param)
{
	struct cyt_timer * timer;
	
	timer = calloc(1, sizeof(struct cyt_timer));
	if (timer == NULL) {
		perror("calloc");
		return NULL;
	}
	
	timer->callback = callback;
	timer->param = param;
	timer->start = start;
	timer->period = period;
	timer->is_running = false;
	
	return timer;
}

void cyt_timer_Destroy(struct cyt_timer * timer)
{
	free(timer);
}

void cyt_timer_Start(struct cyt_timer * timer)
{
	cyt_systick_RegisterTimer(timer);
}

void cyt_timer_Cancel(struct cyt_timer * timer)
{
	cyt_systick_UnregisterTimer(timer);
}

