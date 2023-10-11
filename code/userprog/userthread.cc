#ifdef CHANGED
#include "system.h"
#include "userthread.h"



int ThreadCreate(void f(void *arg), void *arg){
    return 1;
}
void ThreadExit(void){
    return;
}

int do_ThreadCreate(int f, int arg){
    Thread *newThread = new Thread("T1");

    newThread->space = currentThread->space;

    


    newThread->Start(StartUserThread, arg);
    return 1;
}

static void StartUserThread(void *schmurtz){

}


#endif  // CHANGED