#ifdef CHANGED
#include "syscall.h"

// teste avec un string de 10 char
// il suffit de regarder s'il y a le meme nombre de characteres que spécifié
int main() {
	int ret = GetInt();
	PutInt(ret);
	return 0;
}
#endif  // CHANGED