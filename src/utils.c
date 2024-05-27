#include "utils.h"

#ifdef UNIX_BUILD
#include <unistd.h>
#elif WINDOWS_BUILD
#include <windows.h>
#endif

void univ_sleep(double seconds) {
    #ifdef UNIX_BUILD
    usleep(seconds * 1e6);
    #elif WINDOWS_BUILD
    Sleep(seconds * 1e3);
    #endif
}