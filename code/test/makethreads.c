#ifdef CHANGED
#include "syscall.h"

void f(void* a) {
	PutString((char*)a);
	ThreadExit();
}

int main() {
	char* a = "super ca va ?";
	int ret = ThreadCreate(f, a);
	// DEBUG('s', "ThreadCreate = %d\n", ret);  // DEMANDER PQ CA BUG
	while (1) {
	}
	return 0;
}

#endif  // CHANGED
