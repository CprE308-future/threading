


struct cyt_context;


struct cyt_context * cyt_context_Create(size_t stack_size, void(*func)(void*), void * arg);
int cyt_context_SaveContext(struct cyt_context * c);
int cyt_context_ResumeContext(struct cyt_context * c);
int cyt_context_SwitchContext(struct cyt_context * from, struct cyt_context * to);
int cyt_context_ResumeContext(struct cyt_context * c);
void cyt_context_CopyContext(struct cyt_context * dest, void* src);
struct cyt_context * cyt_context_CreateMain();
