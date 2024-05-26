#include "sleep.h"

#include "stdio.h"

#ifdef UNIX_BUILD
#include <unistd.h>
#elif WINDOWS_BUILD
#include <windows.h>
#endif

void sleep(int useconds) {
    #ifdef UNIX_BUILD
    usleep(useconds);
    #elif WINDOWS_BUILD
    Sleep(useconds / 1000);
    #endif
    printf("\n\nhello world\n\n\n");
}