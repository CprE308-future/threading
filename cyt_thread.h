


#ifndef CYT_THREAD_H
#define CYT_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "cyt_types.h"
#include "cyt_llist.h"

typedef void*(*thread_func)(void*);


struct cyt_thread * cyt_thread_Create(thread_func f, void * param, const char * name);
void cyt_thread_Suspend(struct cyt_thread * t);
void cyt_thread_Resume(struct cyt_thread * t);

char const * cyt_thread_GetName(struct cyt_thread* t) __attribute__((pure,returns_nonnull));

#ifdef __cplusplus
}
#endif

#endif

