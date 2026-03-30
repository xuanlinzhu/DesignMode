#include "VoltageManager.h"

extern int printf(const char *format, ...);

void VoltageManager_SetVoltage(float voltage)
{
    printf("[Voltage] %.2fV\n", voltage);
}
