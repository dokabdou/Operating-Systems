#ifdef CHANGED
#include "syscall.h"

// dans le fichier code/userprog faire : ./nachos -d s -x ../test/getint

// le test ne prend pas d'argument et il attend que l'utilisateur entre
// un entier puis il l'affiche avec putint().
// il suffit de regarder si l'entier est bien celui attendu et que les appels systemes
// on bien été effectués
int main() {
	int ret = GetInt();
	PutInt(ret);
	return 0;
}
#endif  // CHANGED