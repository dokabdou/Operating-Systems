#ifdef CHANGED
#include "syscall.h"

void print(char c, int n) {
	int i;

	for (i = 0; i < n; i++) {
		PutChar(c + (i % 26));
	}
	PutChar('\n');
}

// sans argument et ca print 30 char
// il suffit de regarder s'il y a le meme nombre de characteres
// que spécifié et que c'est l'ordre de l'alphabet
int main() {
	print('a', 30);
	return 12;
}
#endif  // CHANGED