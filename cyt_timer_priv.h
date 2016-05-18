



#ifndef CYT_TIMER_PRIV_H
#define CYT_TIMER_PRIV_H

#include <stdbool.h>

#include "cyt_types.h"
#include "cyt_timer.h"
#include "cyt_llist.h"

struct cyt_timer
{
	LLIST(struct cyt_timer, list);

	timer_callback callback;
	void * param;
	jiffy_t start;
	jiffy_t period;
	jiffy_t next;
	bool is_running;
};



#endif

