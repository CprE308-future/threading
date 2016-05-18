


#ifndef CYT_SYSTICK_H
#define CYT_SYSTICK_H

#include "cyt_types.h"

struct cyt_timer;



// initialize the system tick timer with the given frequency
int cyt_systick_Init(unsigned long hz);

// start the sys tick timer
int cyt_systick_Start();

// get the system time in jiffies since the init was called
unsigned long long cyt_systick_GetSysTime();

// register a new timer
void cyt_systick_RegisterTimer(struct cyt_timer * timer);

// unregister a timer
void cyt_systick_UnregisterTimer(struct cyt_timer * timer);

#endif




