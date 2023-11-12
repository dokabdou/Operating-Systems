#ifdef CHANGED
#include "userthread.h"
#include "syscall.h"
#include "system.h"

typedef struct {
	/*strucure that will be sent to StartUserThread*/
	int f;
	int arg;
	int met;
} Args;

static void StartUserThread(void* schmurtz) {
	// copies info from the struct given

	Args* struct_infos = (Args*)schmurtz;

	int f = struct_infos->f;
	int arg = struct_infos->arg;
	int met = struct_infos->met;

	// WriteRegisters()
	int i;

	for (i = 0; i < NumTotalRegs; i++)
		machine->WriteRegister(i, 0);

	machine->WriteRegister(PCReg, f);
	machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
	machine->WriteRegister(StackReg, met);
	machine->WriteRegister(4, arg);

	// MachineRun()
	machine->DumpMem("threads.svg");
	machine->Run();
}

int do_ThreadCreate(int f, int arg) {
	// should call StartUserThread()
	// creates a structure to store  and send to StartUserThread()

	if (currentThread->space->AllocateUserStack() == -1) {
		// There is no space left to add a thread or there is no space left at all
		return -1;
	}

	char* name = new char[64];
	int tc = currentThread->space->ThreadCounterInc();
	sprintf(name, "Thread %d", tc);

	Thread* newThread = new Thread(name);
	newThread->space = currentThread->space;  // giving the child thread the same space as the parent thread
	// thread will be in the same address space

	Args* args = (Args*)malloc(sizeof(Args));
	args->f = f;
	args->arg = arg;
	args->met = currentThread->space->AllocateUserStack();

	newThread->Start(StartUserThread, args);

	// check if the thread was correctly created, return -1 if not
	return 0;
}

void do_ThreadExit() {
	const char* name = currentThread->getName();
	DEBUG('e', "\n%s stopped\n", name);
	int tc = currentThread->space->ThreadCounterDec();

	if (tc == 0) {
		interrupt->Powerdown();
	}
	currentThread->Finish();
}

#endif  // CHANGED