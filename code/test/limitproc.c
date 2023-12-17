#include "syscall.h"

/***
 * Le programme ci-dessous permet de tester les limitations au niveau de la création de processus et de threads.
 * On crée 15 processus, chacun d'eux crée 20 threads. Chaque thread affiche 31 caractères (1 'x' et 30 'a').
 * Pour verifier que le programme fonctionne correctement, on doit avoir 15*20*31 = 9300 caractères affichés.
 */
main() {
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ForkExec("/home/nessar/projet-05/code/test/limitthread");
	ThreadExit();
}