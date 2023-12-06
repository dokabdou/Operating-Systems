#include "syscall.h"

void f(void* c) {
	volatile int i = 0;
	while (i < 100) {
		char* ch = (char*)c;
		PutChar(*ch);
		i++;
	}
	PutInt(i);
	ThreadExit();
}

/**
 * ./nachos -rs 45 -x ../test/makethreads
 * ./nachos -rs 1234 -x ../test/makethreads
 * ./nachos -rs .... -x ../test/makethreads
 * il faut utilisé -rs pour forcer l’ordonnancement préemptif des threads
 */
int main() {
	char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
	ThreadCreate(f, &a);
	ThreadCreate(f, &b);
	ThreadCreate(f, &c);
	ThreadCreate(f, &d);
	ThreadCreate(f, &e);  // strange behavior: this thread is created but never executed probably
	ThreadExit();
	while(1);
	//return 0;
}
