// machine.cc 
//	Routines for simulating the execution of user programs.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "machine.h"
#include "system.h"

// Textual names of the exceptions that can be generated by user program
// execution, for debugging.
static const char* exceptionNames[] = { "no exception", "syscall", 
				"page fault/no TLB entry", "page read only",
				"bus error", "address error", "overflow",
				"illegal instruction" };

//----------------------------------------------------------------------
// CheckEndian
// 	Check to be sure that the host really uses the format it says it 
//	does, for storing the bytes of an integer.  Stop on error.
//----------------------------------------------------------------------

static
void CheckEndian()
{
    union checkit {
        char charword[4];
        unsigned int intword;
    } check;

    check.charword[0] = 1;
    check.charword[1] = 2;
    check.charword[2] = 3;
    check.charword[3] = 4;

#ifdef HOST_IS_BIG_ENDIAN
    ASSERT (check.intword == 0x01020304);
#else
    ASSERT (check.intword == 0x04030201);
#endif
}

//----------------------------------------------------------------------
// Machine::Machine
// 	Initialize the simulation of user program execution.
//
//	"debug" -- if TRUE, drop into the debugger after each user instruction
//		is executed.
//----------------------------------------------------------------------

Machine::Machine(bool debug)
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        registers[i] = 0;
    mainMemory = new char[MemorySize];
    for (i = 0; i < MemorySize; i++)
      	mainMemory[i] = 0;
#ifdef USE_TLB
    tlb = new TranslationEntry[TLBSize];
    for (i = 0; i < TLBSize; i++)
	tlb[i].valid = FALSE;
    pageTable = NULL;
#else	// use linear page table
    tlb = NULL;
    pageTable = NULL;
#endif

    singleStep = debug;
    runUntilTime = 0;
    CheckEndian();
}

//----------------------------------------------------------------------
// Machine::~Machine
// 	De-allocate the data structures used to simulate user program execution.
//----------------------------------------------------------------------

Machine::~Machine()
{
    delete [] mainMemory;
    if (tlb != NULL)
        delete [] tlb;
}

//----------------------------------------------------------------------
// Machine::RaiseException
// 	Transfer control to the Nachos kernel from user mode, because
//	the user program either invoked a system call, or some exception
//	occured (such as the address translation failed).
//
//	"which" -- the cause of the kernel trap
//	"badVaddr" -- the virtual address causing the trap, if appropriate
//----------------------------------------------------------------------

void
Machine::RaiseException(ExceptionType which, int badVAddr)
{
    enum MachineStatus oldStatus = interrupt->getStatus();
    DEBUG('m', "Exception: %s\n", exceptionNames[which]);

    registers[BadVAddrReg] = badVAddr;
    DelayedLoad(0, 0);			// finish anything in progress
    interrupt->setStatus(SystemMode);
    ExceptionHandler(which);		// interrupts are enabled at this point
    interrupt->setStatus(oldStatus);
}

//----------------------------------------------------------------------
// Machine::Debugger
// 	Primitive debugger for user programs.  Note that we can't use
//	gdb to debug user programs, since gdb doesn't run on top of Nachos.
//	It could, but you'd have to implement *a lot* more system calls
//	to get it to work!
//
//	So just allow single-stepping, and printing the contents of memory.
//----------------------------------------------------------------------

void Machine::Debugger()
{
    char *buf = new char[80];
    int num;

    interrupt->DumpState();
    DumpState();

    // LB: Update the print format after the promotion of tick types
    // from int to long long
    //    printf("%d> ", stats->totalTicks);
    printf("%lld> ", stats->totalTicks);
    // End of correction 

    fflush(stdout);
    fgets(buf, 80, stdin);
    if (sscanf(buf, "%d", &num) == 1)
	runUntilTime = num;
    else {
	runUntilTime = 0;
	switch (*buf) {
	  case '\n':
	    break;
	    
	  case 'c':
	    singleStep = FALSE;
	    break;
	    
	  case '?':
	    printf("Machine commands:\n");
	    printf("    <return>  execute one instruction\n");
	    printf("    <number>  run until the given timer tick\n");
	    printf("    c         run until completion\n");
	    printf("    ?         print help message\n");
	    break;
	}
    }
    delete [] buf;
}
 
//----------------------------------------------------------------------
// Machine::DumpState
// 	Print the user program's CPU state.  We might print the contents
//	of memory, but that seemed like overkill.
//----------------------------------------------------------------------

void
Machine::DumpState()
{
    int i;
    
    printf("Machine registers:\n");
    for (i = 0; i < NumGPRegs; i++)
	switch (i) {
	  case StackReg:
	    printf("\tSP(%d):\t0x%x%s", i, registers[i],
		   ((i % 4) == 3) ? "\n" : "");
	    break;
	    
	  case RetAddrReg:
	    printf("\tRA(%d):\t0x%x%s", i, registers[i],
		   ((i % 4) == 3) ? "\n" : "");
	    break;
	  
	  default:
	    printf("\t%d:\t0x%x%s", i, registers[i],
		   ((i % 4) == 3) ? "\n" : "");
	    break;
	}
    
    printf("\tHi:\t0x%x", registers[HiReg]);
    printf("\tLo:\t0x%x\n", registers[LoReg]);
    printf("\tPC:\t0x%x", registers[PCReg]);
    printf("\tNextPC:\t0x%x", registers[NextPCReg]);
    printf("\tPrevPC:\t0x%x\n", registers[PrevPCReg]);
    printf("\tLoad:\t0x%x", registers[LoadReg]);
    printf("\tLoadV:\t0x%x\n", registers[LoadValueReg]);
    printf("\n");
}

//----------------------------------------------------------------------
// DumpReg
// 	Draw a pointer register in the virtual address space
//----------------------------------------------------------------------

void
Machine::DumpReg(FILE *output, int val, const char *name, const char *color,
		 int ptr_x, int ptr_y, unsigned virtual_x,
		 unsigned y, unsigned blocksize)
{
    unsigned page = val / PageSize;
    unsigned offset = val % PageSize;

    fprintf(output, "<text x=\"%d\" y=\"%u\" stroke=\"%s\" fill=\"%s\" font-size=\"%u\">%s</text>\n",
	    ptr_x, y - page * blocksize, color, color, blocksize, name);
    fprintf(output, "<line x1=\"%d\" y1=\"%u\" x2=\"%u\" y2=\"%u\" "
		    "stroke=\"#808080\" stroke-width=\"1\"/>\n",
		    ptr_x + 3*blocksize/2,
		    ptr_y - page * blocksize - blocksize/2,
		    virtual_x + offset * blocksize + blocksize/2,
		    ptr_y - page * blocksize - blocksize/2);
}

//----------------------------------------------------------------------
// DumpRegs
// 	Draw machine pointer registers in the virtual address space
//----------------------------------------------------------------------

void
Machine::DumpRegs(FILE *output, int ptr_x, int ptr_y, unsigned virtual_x,
		  unsigned y, unsigned blocksize)
{
    DumpReg(output, registers[PCReg], "PC", "#FF0000", ptr_x, ptr_y, virtual_x, y, blocksize);
    DumpReg(output, registers[StackReg], "SP", "#FF0000", ptr_x, ptr_y, virtual_x, y, blocksize);
}

//----------------------------------------------------------------------
// PageTableRoom
// 	Return how much room would be needed for showing this page table
//----------------------------------------------------------------------

unsigned
Machine::PageTableRoom(unsigned numPages, unsigned blocksize)
{
    return (numPages+1) * blocksize;
}

//----------------------------------------------------------------------
// get_RGB
// 	Turn a byte into a representative color of the byte
//----------------------------------------------------------------------

static void
get_RGB(unsigned char value, unsigned char *r, unsigned char *g, unsigned char *b)
{
    *r = (value & 0x7) << 5;
    *g = (value & 0x38) << 2;
    *b = (value & 0xc0) << 0; // Humans don't see blue that well
}

//----------------------------------------------------------------------
// DumpPageTable
// 	Draw a page table and its mapping to physical address space
//----------------------------------------------------------------------

unsigned
Machine::DumpPageTable(FILE *output,
		       TranslationEntry *_pageTable, unsigned _pageTableSize,
		       unsigned addr_x, unsigned virtual_x, unsigned virtual_width,
		       unsigned physical_x, unsigned virtual_y, unsigned y,
		       unsigned blocksize)
{
    unsigned page, offset;

    for (page = 0; page < _pageTableSize; page++) {
	TranslationEntry *e = &_pageTable[page];

	fprintf(output, "<text x=\"%u\" y=\"%u\" font-size=\"%u\">0x%04x</text>\n",
		addr_x, virtual_y - page * blocksize, blocksize, page * PageSize);

	if (e->valid) {
	    for (offset = 0; offset < PageSize; offset++) {
		int value;
		unsigned char r, g, b;
		int virt = page * PageSize + offset;
		int phys;

		TranslationEntry *save_pageTable = pageTable;
		unsigned save_pageTableSize = pageTableSize;

		pageTable = _pageTable;
		pageTableSize = _pageTableSize;

		ExceptionType res = Translate(virt, &phys, 1, 0);
		if (res == NoException)
			ReadMem(virt, 1, &value);
		else
			value = -1;

		pageTable = save_pageTable;
		pageTableSize = save_pageTableSize;

		get_RGB(value, &r, &g, &b);

		fprintf(output, "<rect x=\"%u\" y=\"%u\" "
				"width=\"%u\" height=\"%u\" "
				"fill=\"#%02x%02x%02x\" "
				"stroke=\"#000000\" "
				"stroke-width=\"1\"/>\n",
				virtual_x + offset * blocksize,
				virtual_y - page * blocksize - blocksize,
				blocksize, blocksize,
				r, g, b);
	    }

	    fprintf(output, "<line x1=\"%u\" y1=\"%u\" "
			    "x2=\"%u\" y2=\"%u\" "
			    "stroke=\"#000000\" "
			    "stroke-width=\"1\"/>\n",
			    virtual_x + virtual_width,
			    virtual_y - page * blocksize - blocksize/2,
			    physical_x,
			    y - e->physicalPage * blocksize - blocksize/2);
	}
    }

    if (_pageTable == pageTable) {
	fprintf(output, "<rect x=\"%u\" y=\"%u\" "
			"width=\"%u\" height=\"%u\" "
			"fill-opacity=\"0.0\" "
			"stroke=\"#FF0000\" "
			"stroke-width=\"2\"/>\n",
			virtual_x,
			virtual_y - _pageTableSize * blocksize,
			virtual_width, _pageTableSize * blocksize);
    }

    return PageTableRoom(_pageTableSize, blocksize);
}

//----------------------------------------------------------------------
// Machine::DumpMem
// 	Draw the user program's memory layout.
//----------------------------------------------------------------------

void
Machine::DumpMem(const char *name)
{
    FILE *output = fopen(name, "w+");

    const unsigned blocksize = 32;

    const unsigned addr_x = 0;
    const unsigned addr_width = 4*blocksize;

    const unsigned ptr_x = addr_x + addr_width;
    const unsigned ptr_width = 6*blocksize;

    const unsigned virtual_x = ptr_x + ptr_width;
    const unsigned virtual_width = PageSize * blocksize;

    const unsigned physical_x = virtual_x + virtual_width + 30 * blocksize;
    const unsigned physical_width = PageSize * blocksize;

    const unsigned width = physical_x + physical_width;
    unsigned height;
    unsigned page, offset;

    unsigned virtual_height = AddrSpacesRoom(blocksize);
    unsigned physical_height = NumPhysPages * blocksize;

    height = virtual_height > physical_height ? virtual_height : physical_height;

    fprintf(output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(output, "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox = \"0 0 %u %u\" version = \"1.1\">\n", width, height);

    fprintf(output, "<rect x=\"%u\" y=\"%u\" "
		    "width=\"%u\" height=\"%u\" "
		    "fill=\"#ffffff\" "
		    "stroke=\"#000000\" "
		    "stroke-width=\"1\"/>\n",
		    virtual_x,
		    height - pageTableSize * blocksize,
		    virtual_width,
		    pageTableSize * blocksize);

    DumpAddrSpaces(output, addr_x, ptr_x, virtual_x, virtual_width, physical_x, height, blocksize);

    for (page = 0; page < NumPhysPages; page++) {
	for (offset = 0; offset < PageSize; offset++) {
	    int value;
	    unsigned char r, g, b;

	    value = machine->mainMemory[page * PageSize + offset];
	    get_RGB(value, &r, &g, &b);

	    fprintf(output, "<rect x=\"%u\" y=\"%u\" "
			    "width=\"%u\" height=\"%u\" "
			    "fill=\"#%02x%02x%02x\" "
			    "stroke=\"#000000\" "
			    "stroke-width=\"1\"/>\n",
			    physical_x + offset * blocksize,
			    height - page * blocksize - blocksize,
			    blocksize, blocksize,
			    r, g, b);
	}
    }

    fprintf(output, "</svg>\n");
    fclose(output);
}

//----------------------------------------------------------------------
// Machine::ReadRegister/WriteRegister
//   	Fetch or write the contents of a user program register.
//----------------------------------------------------------------------

int Machine::ReadRegister(int num)
    {
	ASSERT((num >= 0) && (num < NumTotalRegs));
	return registers[num];
    }

void Machine::WriteRegister(int num, int value)
    {
	ASSERT((num >= 0) && (num < NumTotalRegs));
	// DEBUG('m', "WriteRegister %d, value %d\n", num, value);
	registers[num] = value;
    }

