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
	console = ...
}
ConsoleDriver::~ConsoleDriver() {
	delete console;
	delete writeDone;
	delete readAvail;
}
4 void ConsoleDriver::PutChar(int ch) {
	// ...
}
int ConsoleDriver::GetChar() {
	// ...
}
void ConsoleDriver::PutString(const char* s) {
	// ...
}
void ConsoleDriver::GetString(char* s, int n) {
	// ...
}
#endif  // CHANGED