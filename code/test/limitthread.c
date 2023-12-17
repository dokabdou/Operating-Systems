#include "syscall.h"
#define THIS "aaa"
const int N = 10;  // Choose it large enough!
void puts(const char* s) {
	const char* p;
	for (p = s; *p != '\0'; p++)
		PutChar(*p);
}
void f(void* arg) {
	const char* s = arg;
	int i;
	PutChar('x');
	for (i = 0; i < N; i++)
		puts(s);
	ThreadExit();
}

/**
 * Ce programme permet de tester les limitations au niveau de la création de threads.
 * On crée 20 threads, chacun d'eux affiche 31 caractères (1 'x' et 30 'a').
 * Pour verifier que le programme fonctionne correctement, on doit avoir 20*31 = 620 caractères affichés.
 */
int main() {
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadCreate(f, THIS);
	ThreadExit();
}