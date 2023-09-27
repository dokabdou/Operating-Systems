#ifdef CHANGED
#include "consoledriver.h"
#include "copyright.h"
#include "synch.h"
#include "system.h"
static Semaphore* readAvail;
static Semaphore* writeDone;
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
	console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}
ConsoleDriver::~ConsoleDriver() {
	delete console;
	delete writeDone;
	delete readAvail;
}
void ConsoleDriver::PutChar(int ch) {
	console->TX(ch);
	writeDone->P();
}
int ConsoleDriver::GetChar() {
	readAvail->P();
	return console->RX();
}
void ConsoleDriver::PutString(const char* s) {
	int c = 0; 
	while(s[c] != '/0') {
		PutChar(s[c]);
        c++;
	}
}

unsigned copyStringFromMachine(int from, char *to, unsigned size) {
	
	unsigned buffer = '/0';	
	unsigned cp = 0;
	ReadMem(from, 1, &buffer);

	while(cp < MAX_STRING_SIZE ){
		to[cp] = buffer;
		cp++;
		if(buffer == '/0') {
			return cp;
		}
	}
	to[cp-1] = '/0';
	return cp;
}
void ConsoleDriver::GetString(char* s, int n) {
	// ...
}
#endif  // CHANGED