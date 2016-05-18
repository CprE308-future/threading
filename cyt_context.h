
#ifndef CYT_CONTEXT_H
#define CYT_CONTEXT_H

#include <sys/types.h>

struct cyt_context;

typedef void(*context_on_start_t)(void*);

struct cyt_context * cyt_context_Create(size_t stack_size, context_on_start_t func, void * arg);
struct cyt_context * cyt_context_CreateMain();
void cyt_context_Delete(struct cyt_context *c);
int cyt_context_SaveContext(struct cyt_context * c);
int cyt_context_ResumeContext(struct cyt_context * c);
int cyt_context_SwitchContext(struct cyt_context * from, struct cyt_context * to);
int cyt_context_ResumeContext(struct cyt_context * c);
void cyt_context_CopyContext(struct cyt_context * dest, void* src);

#endif

