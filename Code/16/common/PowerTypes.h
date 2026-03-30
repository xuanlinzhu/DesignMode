#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    POWER_MODE_OFF = 0,
    POWER_MODE_STANDARD,
    POWER_MODE_PERFORMANCE,
    POWER_MODE_LOW_POWER
} PowerMode;

typedef struct {
    PowerMode mode;
    int freq;
    float voltage;
    int fan;
    const char *policy;
} PowerStatus;

#ifdef __cplusplus
}
#endif
