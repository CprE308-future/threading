
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ucontext.h>

#include "cyt_context.h"

struct cyt_context 
{
	ucontext_t ucontext;
};

struct cyt_context * cyt_context_Create(size_t stack_size, void(*func)(void*), void * arg)
{
	struct cyt_context * c;
	char * stack;
	int rv;

	// allocate the context object
	c = malloc(sizeof(struct cyt_context));
	if(!c){
		perror("malloc");
		goto cleanup;
	}
	
	// allocate a stack for the new context
	stack = malloc(stack_size);
	if(!stack){
		perror("malloc");
		goto cleanup;
	}
	
	// get the current context to modify and make the new
	rv = cyt_context_SaveContext(c);
	if (rv)
		goto cleanup;
	
	// set the stack in the context
	c->ucontext.uc_stack.ss_sp = stack; // set the stack pointer
	c->ucontext.uc_stack.ss_size = stack_size; // set the stack size_t
	c->ucontext.uc_link = NULL; // no link is set.  if this context is ever returned out of it will crash
	
	// make the new context
	makecontext(&c->ucontext, func, 1, arg);
	
	// return the new context
	return c;
	
	// cleanup allocated memory if something fails
	cleanup:
	{
		if(c)
			free(c);
		if(stack)
			free(stack);
		
		return NULL;
	}
}

struct cyt_context * cyt_context_CreateMain()
{
	struct cyt_context * c;
	int rv;

	// allocate the context object
	c = malloc(sizeof(struct cyt_context));
	if(!c){
		perror("malloc");
		return NULL;
	}
	
	cyt_context_SaveContext(c);
	return c;
}

int cyt_context_SaveContext(struct cyt_context * c)
{
	int rv = getcontext(&c->ucontext);
	if(rv < 0){
		perror("getcontext");
		return -1;
	}
	return 0;
}

int cyt_context_ResumeContext(struct cyt_context * c)
{
	int rv = setcontext(&c->ucontext);
	if(rv < 0){
		perror("setcontext");
		return -1;
	}
	return 0;
}

void cyt_context_CopyContext(struct cyt_context *dest, void* src)
{
	memcpy(&dest->ucontext, src, sizeof(ucontext_t));
}

int cyt_context_SwitchContext(struct cyt_context * from, struct cyt_context * to)
{
	int rv = swapcontext(&from->ucontext, &to->ucontext);
	if(rv < 0){
		perror("swapcontext");
		return -1;
	}
	return 0;
}

