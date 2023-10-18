#ifdef CHANGED
#include "system.h"
#include "userthread.h"
#include "syscall.h"


typedef struct
{
    /*strucure that will be sent to StartUserThread*/
    int f;
    int arg;
    int met;
} Args;



int ThreadCreate(void f(void *arg), void *arg){
    return 1;
}
void ThreadExit(void){
    return;
}

int do_ThreadCreate(int f, int arg){
    // should call StartUserThread()
    // creates a structure to store  and send to StartUserThread()
    Thread *newThread = new Thread("T1");

    newThread->space = currentThread->space; // giving the child thread the same space as the parent thread

    if(currentThread->space->AllocateUserStack() == -1){
        // There is no space left to add a thread or there is no space left at all
        return -1;
    }
    // thread will be in the same address space

    Args* args = (Args*) malloc(sizeof(Args));
    args->f = f;
    args->arg = arg;
    args->met = currentThread->space->AllocateUserStack();

    newThread->Start(StartUserThread, args);

    //check if the thread was correctly created, return -1 if not
    return 0;
}

static void StartUserThread(void *schmurtz){
    //copies info from the struct given

    Args* struct_infos = (Args *)schmurtz;

    int f = struct_infos->f;
    int arg = struct_infos->arg;
    int met = struct_infos->met;

    //WriteRegisters()
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister (i, 0);

    machine->WriteRegister (PCReg, f);
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister (StackReg, met); 
    machine->WriteRegister (4, arg); 

    //MachineRun()
    machine->Run();

}


void do_ThreadExit(){
    currentThread->Finish();
    //currentThread->space = 
}

#endif  // CHANGED