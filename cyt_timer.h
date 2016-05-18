


#ifndef CYT_TIMER_H
#define CYT_TIMER_H

#include "cyt_types.h"

struct cyt_timer;

typedef void (*timer_callback)(jiffy_t sys_time, void *);
struct cyt_timer * cyt_timer_Create(jiffy_t start, jiffy_t period, timer_callback callback, void * param);
void cyt_timer_Destroy(struct cyt_timer * timer);
void cyt_timer_Start(struct cyt_timer * timer);
void cyt_timer_Cancel(struct cyt_timer * timer);




#endif


