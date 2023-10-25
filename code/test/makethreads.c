#ifdef CHANGED
#include "syscall.h"

void f(void* a) {
	PutString(a);
	ThreadExit();
}

int main() {
	int ret = ThreadCreate(f, "test");
	ThreadExit();
	DEBUG('s', "ThreadCreate = %d\n", ret);
	return 0;
}

#endif  // CHANGED
