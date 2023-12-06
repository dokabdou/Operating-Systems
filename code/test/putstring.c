#ifdef CHANGED
#include "syscall.h"

// dans le fichier code/userprog faire : ./nachos -d s -x ../test/putstring

/// @brief fillString remplit une chaine de caracteres avec des lettres de l'alphabet
/// @param toTest chaine de caracteres a remplir préalablement allouée
/// @param size taille de la chaine de caracteres
void fillString(char* toTest, int size) {
	int i;
	for (i = 0; i < size; i++) {
		toTest[i] = 'a' + i % 26;
	}
	toTest[size] = '\0';
}

// ce test verifie que putstring arrive a ecrire une chaine de caracteres dans la console
// il suffit de regarder si la chaine est bien celle attendue et que les appels systemes
// on bien été effectués
int main() {
	char toTest[20];
	fillString(toTest, 20);
	PutString(toTest);
	Halt();
	while(1);
	//return 0;
}
#endif  // CHANGED