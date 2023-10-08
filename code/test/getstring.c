#ifdef CHANGED
#include "syscall.h"

// dans le fichier code/userprog faire : ./nachos -d s -x ../test/getstring

// ce test verifie que getstring arrive a lire une chaine de caracteres dans la console
// pour cela on demande a l'utilisateur de rentrer une chaine de caracteres
// puis on affiche cette chaine de caracteres avec putstring()
// il suffit de regarder si la chaine est bien celle attendue et que les appels systemes
// on bien été effectués

int main() {
	char toTest[10];
	GetString(toTest, 10);
	PutString(toTest);
	Halt();
	return 0;
}
#endif  // CHANGED