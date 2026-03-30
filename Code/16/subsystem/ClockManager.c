#include "ClockManager.h"

extern int printf(const char *format, ...);

void ClockManager_SetFrequency(int mhz)
{
    printf("[Clock] %d MHz\n", mhz);
}
