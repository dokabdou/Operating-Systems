#ifdef CHANGED
#include "syscall.h"

void f(void* a) {
	PutString(a);
	ThreadExit();
}

int main() {
	ThreadCreate(f, "test");
	PutString("main");
	ThreadExit();
	return 0;
}

#endif  // CHANGED
