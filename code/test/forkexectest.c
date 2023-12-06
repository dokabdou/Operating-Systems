#ifdef CHANGED
#include "syscall.h"

main()
{
    ForkExec("../test/putchar");
    ForkExec("../test/putchar");
    return 0;
    //while(1);
}
#endif // CHANGED