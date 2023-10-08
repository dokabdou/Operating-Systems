#ifdef CHANGED
#include "syscall.h"

// dans le fichier code/userprog faire : ./nachos -d s -x ../test/putint

// ce test verifie que putint arrive a ecrire un entier dans la console
// il suffit de regarder si le nombre est bien celui attendu et que les appels systemes
// on bien été effectués
int main() {
	PutInt(42);
	return 0;
}
#endif  // CHANGED