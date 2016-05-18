

#ifndef CYT_THREAD_PRIV_H
#ifndef CYT_PRIVATE
 #error Cannot include cyt_thread_priv.h from outside the project.  Please include cyt.h.
#else
#define CYT_THREAD_PRIV_H

struct cyt_thread
{
	//llist queue;
	LLIST(struct cyt_thread, queue);
	unsigned int id;
	thread_func function;
	void* param;
	struct cyt_mutex *mutex_list;
	struct cyt_sem *sem_list;
	struct cyt_context * context;
	struct cyt_thread_info
	{
		char * name;
		//clock_t arrive_time;
	} info;
	LLIST(struct cyt_thread, siblings);
	struct cyt_thread * parent;
};


#endif
#endif
