#include "SchedulerManager.h"

extern int printf(const char *format, ...);

void SchedulerManager_SetPolicy(SchedulerPolicy policy)
{
    const char *label;

    switch (policy) {
    case SCHED_POLICY_PERFORMANCE:
        label = "PERFORMANCE";
        break;
    case SCHED_POLICY_BALANCE:
        label = "BALANCE";
        break;
    case SCHED_POLICY_POWER_SAVE:
        label = "POWER_SAVE";
        break;
    case SCHED_POLICY_STOP:
    default:
        label = "STOP";
        break;
    }

    printf("[Scheduler] %s\n", label);
}
