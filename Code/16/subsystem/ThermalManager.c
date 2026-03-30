#include "ThermalManager.h"

extern int printf(const char *format, ...);

void ThermalManager_SetFan(FanMode mode)
{
    const char *label;

    switch (mode) {
    case FAN_MODE_HIGH:
        label = "FAN HIGH";
        break;
    case FAN_MODE_LOW:
        label = "FAN LOW";
        break;
    case FAN_MODE_OFF:
    default:
        label = "FAN OFF";
        break;
    }

    printf("[Thermal] %s\n", label);
}
