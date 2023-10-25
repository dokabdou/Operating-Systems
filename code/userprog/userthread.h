#ifdef CHANGED

int ThreadCreate(void f(void *arg), void *arg);
void ThreadExit(void);
extern int do_ThreadCreate(int f, int arg);
static void StartUserThread(void *schmurtz);
void do_ThreadExit();

#endif  // CHANGED