
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cyt.h"
#include "cyt_timer.h"

void* t1(void* p)
{
	long long i;
	printf("T1 starting\n");
	for(i=0;i<10000000000;i++);
	printf("T1 Stoping\n");
	return NULL;
}

void* t2(void* p)
{
	long long i;
	printf("T2 starting\n");
	for(i=0;i<10000000000;i++);
	printf("T2 Stoping\n");
	return NULL;
}

void* t3(void* p)
{
	long long i;
	printf("T3 starting\n");
	for(i=0;i<10000000000;i++);
	printf("T3 Stoping\n");
	return NULL;
}

void t4(jiffy_t time, void * p)
{
	printf("timer %llu\n",time);
}

extern struct scheduler const * rr_init();

int main(int argc, const char * argv[])
{
	//init();
	cyt_Init(rr_init());
	
	printf("Creating thread T1\n");
	cyt_thread_Create(t1,NULL,"T1");
	printf("Creating thread T2\n");
	cyt_thread_Create(t2,NULL,"T2");
	printf("Creating thread T3\n");
	cyt_thread_Create(t3,NULL,"T3");
	printf("Finished creating threads, goodnight\n");

	//struct cyt_timer * t = cyt_timer_Create(0, 5, t4, NULL);
	//cyt_timer_Start(t);
	
	cyt_thread_Suspend(NULL);
	printf("Something woke me up?\n");
	
	//while(1);
	
	
	return 0;
}

