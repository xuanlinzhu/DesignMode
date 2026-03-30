#pragma once

typedef enum {
    FAN_MODE_OFF = 0,
    FAN_MODE_LOW,
    FAN_MODE_HIGH
} FanMode;

#ifdef __cplusplus
extern "C" {
#endif

void ThermalManager_SetFan(FanMode mode);

#ifdef __cplusplus
}
#endif
