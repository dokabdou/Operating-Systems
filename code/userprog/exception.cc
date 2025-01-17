// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "syscall.h"
#include "system.h"
#ifdef CHANGED
#include "forkexec.h"
#include "userthread.h"
#endif  // CHANGED

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void UpdatePC() {
	int pc = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, pc);
	pc = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, pc);
	pc += 4;
	machine->WriteRegister(NextPCReg, pc);
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which) {
	int type = machine->ReadRegister(2);
	int address = machine->ReadRegister(BadVAddrReg);

	switch (which) {
		case SyscallException: {
			switch (type) {
				case SC_Halt: {
					DEBUG('s', "Shutdown, initiated by user program.\n");
					interrupt->Powerdown();
					break;
				}
#ifdef CHANGED
				case SC_PutChar: {
					DEBUG('s', "PutChar called.\n");
					char c = machine->ReadRegister(4);
					consoledriver->PutChar(c);
					break;
				}
				case SC_PutString: {
					DEBUG('s', "PutString called.\n");
					int from = machine->ReadRegister(4);
					char* buffer = new char[MAX_STRING_SIZE];
					consoledriver->copyStringFromMachine(from, buffer, MAX_STRING_SIZE);
					consoledriver->PutString(buffer);
					delete[] buffer;
					break;
				}
				case SC_Exit: {
					DEBUG('s', "Exit called.\n");
					int status = machine->ReadRegister(2);
					DEBUG('s', "Exit status: %d\n", status);
					int pc = machine->ProcessCounterDec();
					if (pc == 0) {
						DEBUG('s', "No more processes are running, shutting down.\n");
						interrupt->Powerdown();
					} else {
						// Free process resources
						DEBUG('s', "Freeing process resources.\n");
						cleanUpProcess();
					}
					break;
				}
				case SC_GetChar: {
					DEBUG('s', "GetChar called.\n");
					int c = consoledriver->GetChar();
					machine->WriteRegister(2, c);
					break;
				}
				case SC_GetString: {
					DEBUG('s', "GetString called.\n");
					int to = machine->ReadRegister(4);
					char* buffer = new char[MAX_STRING_SIZE];
					consoledriver->GetString(buffer, MAX_STRING_SIZE);
					consoledriver->copyStringToMachine(buffer, to, MAX_STRING_SIZE);
					delete[] buffer;
					break;
				}
				case SC_PutInt: {
					DEBUG('s', "PutInt called.\n");
					int n = machine->ReadRegister(4);
					char* buffer = new char[MAX_STRING_SIZE];
					snprintf(buffer, MAX_STRING_SIZE, "%d", n);
					consoledriver->PutString(buffer);
					delete[] buffer;
					break;
				}
				case SC_GetInt: {
					DEBUG('s', "GetInt called.\n");
					int n = 0;
					char* buffer = new char[MAX_STRING_SIZE];
					consoledriver->GetString(buffer, MAX_STRING_SIZE);
					sscanf(buffer, "%d", &n);
					machine->WriteRegister(2, n);
					delete[] buffer;
					break;
				}
				case SC_ThreadCreate: {
					DEBUG('s', "ThreadCreate called.\n");
					int t = do_ThreadCreate(machine->ReadRegister(4), machine->ReadRegister(5));
					machine->WriteRegister(2, t);  // ??
					break;
				}

				case SC_ThreadExit: {
					do_ThreadExit();
					break;
				}

				case SC_ForkExec: {
					DEBUG('s', "ForkExec called.\n");
					int from = machine->ReadRegister(4);
					char* buffer = new char[MAX_STRING_SIZE];
					consoledriver->copyStringFromMachine(from, buffer, MAX_STRING_SIZE);

					int t = do_ForkExec(buffer);
					machine->WriteRegister(2, t);
					// DEBUG('s', "ForkExec finished.\n");
					break;
				}

#endif  // CHANGED
				default: {
					ASSERT_MSG(FALSE, "Unimplemented system call %d\n", type);
				}
			}

			// Do not forget to increment the pc before returning!
			// This skips over the syscall instruction, to continue execution
			// with the rest of the program
			UpdatePC();
			break;
		}

		case PageFaultException:
			if (!address) {
				ASSERT_MSG(FALSE, "NULL dereference at PC %x!\n", machine->registers[PCReg]);
			} else {
				// For now
				ASSERT_MSG(FALSE, "Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
			}
			break;

		case ReadOnlyException:
			// For now
			ASSERT_MSG(FALSE, "Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
			break;

		case BusErrorException:
			// For now
			ASSERT_MSG(FALSE, "Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
			break;

		case AddressErrorException:
			// For now
			ASSERT_MSG(FALSE, "Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
			break;

		case OverflowException:
			// For now
			ASSERT_MSG(FALSE, "Overflow at PC %x\n", machine->registers[PCReg]);
			break;

		case IllegalInstrException:
			// For now
			ASSERT_MSG(FALSE, "Illegal instruction at PC %x\n", machine->registers[PCReg]);
			break;

		default:
			ASSERT_MSG(FALSE, "Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
			break;
	}
}
