#ifdef CHANGED
#include "syscall.h"

// teste avec un string de 10 char
// il suffit de regarder s'il y a le meme nombre de characteres que spécifié
int main() {
	char toTest[10];
	GetString(toTest, 10);
	PutString(toTest);
	Halt();
	return 0;
}
#endif  // CHANGED