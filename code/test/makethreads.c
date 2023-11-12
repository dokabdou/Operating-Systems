#ifdef CHANGED
#include "syscall.h"


void f(char c) {
	//void *a
	//PutString((char *) a);
	//PutChar(a);//does nothing, before lock implementation : is it not supposed to work, 
	//a is not printed in the terminal like with PutString

	// f(char c) and PutString(a); = trying it with char creates an error : 
	// Assertion FALSE failed: line 170, file "exception.cc:170"
	// Page Fault at address 61 at PC cc	
	
	while(1){
		PutChar(c);
	}
	ThreadExit();
}

void g(char c){
	//PutString((char *) a); 
	// g(char c) and PutString(a); = trying it with char creates an error : 
	// Assertion FALSE failed: line 170, file "exception.cc:170"
	// Page Fault at address 61 at PC cc	
	while(1){
		PutChar(c);
	}
	ThreadExit();
}

int main() {
	//char* a = "super ca va ?";
	//char* b = "super aussi !";
	ThreadCreate(f, 'a'); // ThreadCreate(f, 'a');
	ThreadCreate(g, 'b'); // ThreadCreate(f, 'b');
	
	// DEBUG('s', "ThreadCreate = %d\n", ret);  // DEMANDER PQ CA BUG
	/*while (1) {
	}*/
	ThreadExit();
	// runs forever eventhough there is no infinite while loop
	// printf a blank line = could represent the main thread
	// then "Thread stopped"
	// then message in a
	// then "Thread stopped"
	// "Thread stopped" is printed 2 times in the terminal"
	
	//printf("\nEnd of main()\n"); // wont work, error : "Implicit declaration of "printf" "
	return 0;
}

#endif  // CHANGED
