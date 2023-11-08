#ifdef CHANGED
#include "syscall.h"

void f(void* a) {
	PutString((char*)a);
	//PutChar(a);//does nothing, before lock implementation : is it not supposed to work, 
	//a is not printed in the terminal like with PutString
	ThreadExit();
}

int main() {
	char* a = "super ca va ?";
	//int a = "s";
	int ret = ThreadCreate(f, a);
	// DEBUG('s', "ThreadCreate = %d\n", ret);  // DEMANDER PQ CA BUG
	/*while (1) {
	}*/
	ThreadExit();
	return 0;
}

#endif  // CHANGED
