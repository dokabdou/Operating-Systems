#ifdef CHANGED

#include "forkexec.h"
#include <consoledriver.h>
#include "addrspace.h"
#include "console.h"
#include "copyright.h"
#include "synch.h"
#include "system.h"


void ForkExec(char *filename) {
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL) {
        printf("Unable to open file %s\n", filename);
        return;
    }
    space = new AddrSpace(executable);
    currentThread->space = space;

    delete executable;  // close file

    space->InitRegisters();  // set the initial register values
    space->RestoreState();   // load page table register

    machine->Run();  // jump to the user progam
    ASSERT(false);  // machine->Run never returns;
                    // the address space exits
                    // by doing the syscall "exit"
}

#endif // CHANGED