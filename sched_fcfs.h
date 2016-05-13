#include <stdio.h>

#include "cyt_scheduler.h"
#include "cyt_thread.h"

static struct cyt_thread* head = NULL;

static struct cyt_thread* fcfs_onArrive(struct cyt_thread* t)
{
	printf("sched: thread %s arrived\n", t->info.name);
	if(head) {
		head->queue.prev->next = t;
		t->queue.prev = head->queue.prev;
		t->queue.next = head;
		head->queue.prev = t;
	} else {
		head = t;
		head->queue.next = head;
		head->queue.prev = head;
	}
	return head;
}

static struct cyt_thread* fcfs_onFinish(struct cyt_thread* t)
{
	printf("sched: thread %s finished\n", t->info.name);
	if(t->queue.next == t) {
		head = NULL;
	} else {
		if(head == t)
			head = t->queue.next;
		t->queue.prev->next = t->queue.next;
		t->queue.next->prev = t->queue.prev;
	}
	return head;
}

static struct cyt_thread* fcfs_onSuspend(struct cyt_thread* t)
{
	printf("sched: thread %s suspended\n", t->info.name);
	if(t->queue.next == t) {
		head = NULL;
	} else {
		if(head == t)
			head = t->queue.next;
		t->queue.prev = t->queue.next;
		t->queue.next = t->queue.prev;
	}
	return head;
}

void print_queue()
{
	struct cyt_thread* t = head;
	printf("Sched Queue: ");
	if(t)
		do {
			printf("%s -> ", t->info.name);
			t = t->queue.next;
		} while (t != head);
	else
		printf("NULL");
	printf("\n");
}

static struct cyt_thread* fcfs_onTick(struct cyt_thread* t, unsigned long time)
{
	printf("sched: onTick at time %lu\n", time);
	print_queue();
	if(head)
		head = head->queue.next;
	return head;
}

static const struct scheduler fcfs =
{
	.OnArrive = fcfs_onArrive,
	.OnFinish = fcfs_onFinish,
	.OnSuspend = fcfs_onSuspend,
	.OnTick = fcfs_onTick
};


struct scheduler const * init()
{
	return &fcfs;
}

