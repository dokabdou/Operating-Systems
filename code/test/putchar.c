#ifdef CHANGED
#include "syscall.h"

void print(char c, int n) {
	int i;

	for (i = 0; i < n; i++) {
		PutChar(c + (i % 26));
	}
	PutChar('\n');
}

// soit sans argument et ca print 30 char soit
// avec un argument et ca print le nombre de char spécifié,
// il suffit de regarder s'il y a le meme nombre de characteres
// que spécifié et que c'est bien l'alphabet
int main(int argc, char** argv) {
	if (argc < 2) {
		print('a', 30);
		Halt();
		return 0;
	}
	int size = atoi(argv[1]);
	print('a', size);
	Halt();
	return 0;
}
#endif  // CHANGED