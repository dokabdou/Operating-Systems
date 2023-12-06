#ifdef CHANGED

#include "forkexec.h"
#include <consoledriver.h>
#include "addrspace.h"
#include "console.h"
#include "copyright.h"
#include "synch.h"
#include "system.h"


static void StartUserProc(void* space) {
    currentThread->space = (AddrSpace*)space;
    currentThread->space->InitRegisters();  // set the initial register values
    currentThread->space->RestoreState();   // load page table register

    machine->Run();  // jump to the user program

}

int do_ForkExec(const char *s) {
    // parent = the process that called ForkExec
    OpenFile *executable = fileSystem->Open(s);
    AddrSpace *space;

    if (executable == NULL) {
        SetColor(stdout, ColorRed);
		SetBold(stdout);
		printf("Unable to open file %s\n", s);
		ClearColor(stdout);
        return -1;
    }

    try {
		space = new AddrSpace(executable);
	} catch (const NoMoreMemory& e) {
        SetColor(stdout, ColorRed);
        SetBold(stdout);
        printf("Not enough memory\n");
        ClearColor(stdout);
        return -1;
	}
    Thread* newThread = new Thread("Thread");

    if(newThread == NULL) {
        SetColor(stdout, ColorRed);
        SetBold(stdout);
        printf("Unable to create thread\n");
        ClearColor(stdout);
        return -1;
    }

    newThread->space = space;

    machine->ProcessCounterInc();

    newThread->Start(StartUserProc, space);
	
    delete executable;  // close file
    return 0;
}




#endif // CHANGED