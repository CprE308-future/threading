


#ifndef CYT_TYPES_H
#define CYT_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif


struct cyt_mutex;
struct cyt_sem;
struct cyt_thread;

// A jiffy is a unit of measurement that is one systimer tick.  Therefore in real time it is 1/HZ.
typedef unsigned long long jiffy_t;


#ifdef __cplusplus
}
#endif

#endif

