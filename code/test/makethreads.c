#ifdef CHANGED
#include "syscall.h"


int f(a){
    PutString(a);
    ThreadExit();
}

int main(){
    ThreadCreate(f, "test");
    PutString("main");
    ThreadExit();
}

#endif  // CHANGED