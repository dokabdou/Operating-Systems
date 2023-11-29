#ifdef CHANGED

#include "forkexec.h"
#include <consoledriver.h>
#include "addrspace.h"
#include "console.h"
#include "copyright.h"
#include "synch.h"
#include "system.h"


void do_ForkExec(char *filename) {
    // parent = the process that called ForkExec
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL) {
        SetColor(stdout, ColorRed);
		SetBold(stdout);
		printf("Unable to open file %s\n", filename);
		ClearColor(stdout);
        return;
    }
    space = new AddrSpace(executable);
    Thread* newThread = new Thread("Thread");
    newThread->space = space;

    newThread->Start(StartUserProc, space);
	
    delete executable;  // close file
}


static void StartUserProc(void* space) {
    ((AddrSpace*)space)->InitRegisters();  // set the initial register values
    ((AddrSpace*)space)->RestoreState();   // load page table register

    machine->Run();  // jump to the user program

}

#endif // CHANGED