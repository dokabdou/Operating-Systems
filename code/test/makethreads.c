#ifdef CHANGED
#include "syscall.h"

void f(void* a) {
	PutChar(*(char*)a);
	ThreadExit();
}

int main() {
	int ret = ThreadCreate(f, "test");
	
    ThreadExit();
	//DEBUG('s', "ThreadCreate = %d\n", ret);
	return 0;
}

#endif  // CHANGED
