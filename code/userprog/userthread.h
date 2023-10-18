#ifdef CHANGED

int ThreadCreate(void f(void *arg), void *arg);
void ThreadExit(void);
extern int do_ThreadCreate(int f, int arg);
void StartUserThread(void *schmurtz);

#endif  // CHANGED