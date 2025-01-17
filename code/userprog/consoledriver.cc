#ifdef CHANGED
#include "consoledriver.h"
#include "copyright.h"
#include "synch.h"
#include "system.h"
static Semaphore* readAvail;
static Semaphore* writeDone;

static Lock* readLock;
static Lock* writeLock;

static void ReadAvailHandler(void* arg) {
	(void)arg;
	readAvail->V();
}
static void WriteDoneHandler(void* arg) {
	(void)arg;
	writeDone->V();
}
ConsoleDriver::ConsoleDriver(const char* in, const char* out) {
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	readLock = new Lock("readLock");
	writeLock = new Lock("writeLock");
	console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}
ConsoleDriver::~ConsoleDriver() {
	delete console;
	delete writeDone;
	delete readAvail;
}
void ConsoleDriver::PutChar(int ch) {
	writeLock->Acquire();
	console->TX(ch);
	writeDone->P();
	writeLock->Release();
}
int ConsoleDriver::GetChar() {
	readLock->Acquire();
	readAvail->P();
	int ret = console->RX();
	readLock->Release();
	return ret;
}
void ConsoleDriver::PutString(const char* s) {
	int c = 0;
	while (s[c] != '\0') {
		PutChar(s[c]);
		c++;
	}
}

unsigned ConsoleDriver::copyStringFromMachine(int from, char* to, unsigned size) {
	int buffer = '\0';
	unsigned cp = 0;
	while (cp < size) {
		machine->ReadMem(from + cp, 1, &buffer);
		to[cp] = buffer;
		cp++;
		if (buffer == '\0')
			return cp;
	}
	to[cp - 1] = '\0';
	return cp;
}

void ConsoleDriver::GetString(char* s, int n) {
	int c = 0;
	int rd;  // read character
	for (; c < n - 1; c++) {
		rd = GetChar();
		if (rd == '\n') {
			// if it is a newline character it puts it copies it to s and then stops the program
			s[c] = rd;
			s[c + 1] = '\0';
			return;
		}
		if (rd == EOF) {
			s[c + 1] = '\0';
			return;
		}
		s[c] = rd;
	}
	// appending a teminating null character after the copied characters
	s[c + 1] = '\0';
}

unsigned ConsoleDriver::copyStringToMachine(char* from, int to, unsigned size) {
	unsigned cp = 0;
	while (cp < size && from[cp] != '\0') {
		machine->WriteMem(to + cp, 1, from[cp]);
		cp++;
	}
	return cp;
}
#endif  // CHANGED