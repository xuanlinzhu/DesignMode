#pragma once

typedef enum {
    PERIPHERAL_PROFILE_OFF = 0,
    PERIPHERAL_PROFILE_MINIMAL,
    PERIPHERAL_PROFILE_NECESSARY,
    PERIPHERAL_PROFILE_ALL
} PeripheralProfile;

#ifdef __cplusplus
extern "C" {
#endif

void PeripheralManager_SetProfile(PeripheralProfile profile);

#ifdef __cplusplus
}
#endif
