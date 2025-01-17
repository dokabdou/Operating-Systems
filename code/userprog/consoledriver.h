#ifdef CHANGED
#ifndef CONSOLEDRIVER_H
#define CONSOLEDRIVER_H
#include "console.h"
#include "copyright.h"
#include "utility.h"
class ConsoleDriver : dontcopythis {
   public:
	// initialize the hardware console device
	ConsoleDriver(const char* readFile, const char* writeFile);
	~ConsoleDriver();                                                   // clean up
	void PutChar(int ch);                                               // Behaves like putchar(3S)
	int GetChar();                                                      // Behaves like getchar(3S)
	void PutString(const char* s);                                      // Behaves like fputs(3S)
	void GetString(char* s, int n);                                     // Behaves like fgets(3S)
	unsigned copyStringFromMachine(int from, char* to, unsigned size);  // copy string from machine to userland
	unsigned copyStringToMachine(char* from, int to, unsigned size);    // copy string from userland to machine
   private:
	Console* console;
};

#endif  // CONSOLEDRIVER_H
#endif  // CHANGED