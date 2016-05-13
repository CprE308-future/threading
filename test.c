
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cyt_types.h"
#include "cyt_thread.h"
#include "sched_fcfs.h"


void* t1(void* p)
{
	printf("T1 starting\n");
	sleep(1);
	printf("T1 Stoping\n");
	return NULL;
}

void* t2(void* p)
{
	printf("T2 starting\n");
	sleep(1);
	printf("T2 Stoping\n");
	return NULL;
}

void* t3(void* p)
{
	printf("T3 starting\n");
	sleep(1);
	printf("T3 Stoping\n");
	return NULL;
}

int main(int argc, const char * argv[])
{
	//init();
	cyt_Init(init());
	
	printf("Creating thread T1\n");
	cyt_thread_Create(t1,NULL,"T1");
	printf("Creating thread T2\n");
	cyt_thread_Create(t2,NULL,"T2");
	printf("Creating thread T3\n");
	cyt_thread_Create(t3,NULL,"T3");
	printf("Finished creating threads, goodnight\n");
	//cyt_thread_Suspend(NULL);
	printf("Something woke me up?\n");
	
	while(1);
	
	
	return 0;
}

