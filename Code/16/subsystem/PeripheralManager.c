#include "PeripheralManager.h"

extern int printf(const char *format, ...);

void PeripheralManager_SetProfile(PeripheralProfile profile)
{
    const char *label;

    switch (profile) {
    case PERIPHERAL_PROFILE_ALL:
        label = "ALL ON";
        break;
    case PERIPHERAL_PROFILE_NECESSARY:
        label = "NECESSARY ON";
        break;
    case PERIPHERAL_PROFILE_MINIMAL:
        label = "MINIMAL ON";
        break;
    case PERIPHERAL_PROFILE_OFF:
    default:
        label = "OFF";
        break;
    }

    printf("[Peripheral] %s\n", label);
}
