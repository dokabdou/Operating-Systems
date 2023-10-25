#ifdef CHANGED
#include "syscall.h"

void f(void* a) {
	PutChar(*(char*)a);
	ThreadExit();
}

int main() {
	int ret = ThreadCreate(f, "t");

	PutChar('m');
	ThreadExit();
	return 0;
}

#endif  // CHANGED
