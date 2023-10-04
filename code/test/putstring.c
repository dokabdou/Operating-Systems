#ifdef CHANGED
#include "syscall.h"

// TODO: changer c'est pas dans la memeoire
void fillString(char* toTest, int size) {
	int i;
	for (i = 0; i < size; i++) {
		toTest[i] = 'a' + i % 26;
	}
	toTest[size] = '\0';
}

// teste avec un string de 10 char
// il suffit de regarder s'il y a le meme nombre de characteres que spécifié
int main() {
	char toTest[10];
	fillString(toTest, 10);
	PutString(toTest);
	Halt();
	return 0;
}
#endif  // CHANGED