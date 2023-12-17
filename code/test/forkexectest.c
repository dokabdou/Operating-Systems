#include "syscall.h"

/**
 * Ce programme permet de tester si on arrive a creer deux processus.
 * La fonction f affiche 31 caractères, l'un affiche 30 'a' et un 'b', les deux écrivent un 'x' au debut.
 * Chaque processus crée un thread qui execute f et il execute f lui meme.
 * Pour verifier que le programme fonctionne correctement, on doit avoir 2*2*31 = 124 caractères affichés.
 */
main() {
	ForkExec("../test/userpages1");
	ForkExec("../test/userpages2");
}