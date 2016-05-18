#include <stdio.h>

#include "cyt_scheduler.h"
#include "cyt_thread.h"

#define CYT_PRIVATE
#include "cyt_thread_priv.h"

#ifdef DEBUG
#define dprintf(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define dprintf(fmt, ...)
#endif

static struct cyt_thread* head = NULL;

static struct cyt_thread* rr_onArrive(struct cyt_thread* t)
{
	dprintf("sched: thread %s arrived\n", cyt_thread_GetName(t));
	if(head) {
		LLIST_INSERT_BEFORE(queue, head, t);
	} else {
		LLIST_INIT(queue, t);
		head = t;
	}
	return head;
}

static struct cyt_thread* rr_onFinish(struct cyt_thread* t)
{
	dprintf("sched: thread %s finished\n", cyt_thread_GetName(t));
	if(t->queue.next == t) {
		head = NULL;
	} else {
		if(head == t)
			head = t->queue.next;
		LLIST_REMOVE(queue, t);
	}
	return head;
}

static struct cyt_thread* rr_onSuspend(struct cyt_thread* t)
{
	dprintf("sched: thread %s suspended\n", cyt_thread_GetName(t));
	if(t->queue.next == t) {
		head = NULL;
	} else {
		if(head == t)
			head = t->queue.next;
		LLIST_REMOVE(queue, t);
	}
	return head;
}

void print_queue()
{
	struct cyt_thread* t = head;
	dprintf("Sched Queue: ");
	if(t)
		do {
			dprintf("%s -> ", t->info.name);
			t = t->queue.next;
		} while (t != head);
	else
		dprintf("NULL");
	dprintf("\n");
}

static struct cyt_thread* rr_onTick(struct cyt_thread* t, unsigned long time)
{
	dprintf("sched: onTick at time %lu\n", time);
	print_queue();
	if(head)
		head = head->queue.next;
	return head;
}

static const struct scheduler rr =
{
	.OnArrive = rr_onArrive,
	.OnFinish = rr_onFinish,
	.OnSuspend = rr_onSuspend,
	.OnResume = rr_onArrive,
	.OnTick = rr_onTick
};

struct scheduler const * rr_init()
//struct scheduler const * init()
{
	return &rr;
}

