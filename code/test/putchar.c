#ifdef CHANGED
#include "syscall.h"

// dans le fichier code/userprog faire : ./nachos -d s -x ../test/putchar

void print(char c, int n) {
	int i;

	for (i = 0; i < n; i++) {
		PutChar(c + (i % 26));
	}
	PutChar('\n');
}

// le test ne prend pas d'argument et il print 30 caracteres
// il suffit de regarder si les caracteres sont bien ceux attendus
// et que les appels systemes on bien été effectués
// ce test verifie bien que putchar arrive a ecrire differents
// caracteres dans la console
int main() {
	print('a', 30);
	return 0;
}
#endif  // CHANGED