#pragma once

typedef enum {
    SCHED_POLICY_STOP = 0,
    SCHED_POLICY_BALANCE,
    SCHED_POLICY_PERFORMANCE,
    SCHED_POLICY_POWER_SAVE
} SchedulerPolicy;

#ifdef __cplusplus
extern "C" {
#endif

void SchedulerManager_SetPolicy(SchedulerPolicy policy);

#ifdef __cplusplus
}
#endif
