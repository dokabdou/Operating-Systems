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

// soit sans argument et ca teste avec un string de 10 char soit
// avec un argument et ca teste avec un string de la taille de l'argument,
// il suffit de regarder s'il y a le meme nombre de characteres que spécifié
int main(int argc, char** argv) {
	if (argc < 2) {
		char toTest[10];
		fillString(toTest, 10);
		PutString(toTest);
		return 0;
	}
	int size = atoi(argv[1]);
	char string[size];
	fillString(string, size);
	PutString(string);
	Halt();
	return 0;
}
#endif  // CHANGED