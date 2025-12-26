#include "../include/sleep.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void crossSleep(unsigned int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds); // Windows uses milliseconds
#else
    usleep(milliseconds * 1000); // Linux/macOS uses microseconds
#endif
}