#include "PowerFacade.h"

#include "../subsystem/ClockManager.h"
#include "../subsystem/PeripheralManager.h"
#include "../subsystem/SchedulerManager.h"
#include "../subsystem/ThermalManager.h"
#include "../subsystem/VoltageManager.h"

extern int printf(const char *format, ...);
extern int puts(const char *str);

static PowerStatus g_status;
static int g_initialized = 0;

static const char *mode_to_string(PowerMode mode)
{
    switch (mode) {
    case POWER_MODE_PERFORMANCE:
        return "PERFORMANCE";
    case POWER_MODE_LOW_POWER:
        return "LOW_POWER";
    case POWER_MODE_OFF:
        return "OFF";
    case POWER_MODE_STANDARD:
    default:
        return "STANDARD";
    }
}

static const char *fan_to_string(int fan)
{
    if (fan >= 2) {
        return "HIGH";
    }
    if (fan == 1) {
        return "LOW";
    }
    return "OFF";
}

static void apply_mode(PowerMode mode)
{
    switch (mode) {
    case POWER_MODE_PERFORMANCE:
        g_status.mode = POWER_MODE_PERFORMANCE;
        g_status.freq = 3200;
        g_status.voltage = 1.20f;
        g_status.fan = 2;
        g_status.policy = "PERFORMANCE";
        VoltageManager_SetVoltage(g_status.voltage);
        ClockManager_SetFrequency(g_status.freq);
        PeripheralManager_SetProfile(PERIPHERAL_PROFILE_ALL);
        ThermalManager_SetFan(FAN_MODE_HIGH);
        SchedulerManager_SetPolicy(SCHED_POLICY_PERFORMANCE);
        break;

    case POWER_MODE_LOW_POWER:
        g_status.mode = POWER_MODE_LOW_POWER;
        g_status.freq = 800;
        g_status.voltage = 0.85f;
        g_status.fan = 1;
        g_status.policy = "POWER_SAVE";
        VoltageManager_SetVoltage(g_status.voltage);
        ClockManager_SetFrequency(g_status.freq);
        PeripheralManager_SetProfile(PERIPHERAL_PROFILE_MINIMAL);
        ThermalManager_SetFan(FAN_MODE_LOW);
        SchedulerManager_SetPolicy(SCHED_POLICY_POWER_SAVE);
        break;

    case POWER_MODE_OFF:
        g_status.mode = POWER_MODE_OFF;
        g_status.freq = 0;
        g_status.voltage = 0.0f;
        g_status.fan = 0;
        g_status.policy = "STOP";
        VoltageManager_SetVoltage(g_status.voltage);
        ClockManager_SetFrequency(g_status.freq);
        PeripheralManager_SetProfile(PERIPHERAL_PROFILE_OFF);
        ThermalManager_SetFan(FAN_MODE_OFF);
        SchedulerManager_SetPolicy(SCHED_POLICY_STOP);
        break;

    case POWER_MODE_STANDARD:
    default:
        g_status.mode = POWER_MODE_STANDARD;
        g_status.freq = 2000;
        g_status.voltage = 1.00f;
        g_status.fan = 1;
        g_status.policy = "BALANCE";
        VoltageManager_SetVoltage(g_status.voltage);
        ClockManager_SetFrequency(g_status.freq);
        PeripheralManager_SetProfile(PERIPHERAL_PROFILE_NECESSARY);
        ThermalManager_SetFan(FAN_MODE_LOW);
        SchedulerManager_SetPolicy(SCHED_POLICY_BALANCE);
        break;
    }
}

void PowerFacade_Init(void)
{
    if (g_initialized != 0) {
        puts("[Facade] Already initialized");
        return;
    }

    g_initialized = 1;
    puts("[Facade] Init power subsystem");
    apply_mode(POWER_MODE_STANDARD);
}

void PowerFacade_SetMode(PowerMode mode)
{
    const char *from;
    const char *to;

    if (g_initialized == 0) {
        PowerFacade_Init();
    }

    from = mode_to_string(g_status.mode);
    to = mode_to_string(mode);
    printf("[Facade] Switch %s -> %s\n", from, to);
    apply_mode(mode);
}

void PowerFacade_ShowStatus(void)
{
    if (g_initialized == 0) {
        PowerFacade_Init();
    }

    printf("Mode: %s\n", mode_to_string(g_status.mode));
    printf("Freq: %d MHz\n", g_status.freq);
    printf("Voltage: %.2f V\n", g_status.voltage);
    printf("Fan: %s\n", fan_to_string(g_status.fan));
    printf("Policy: %s\n", g_status.policy);
}

void PowerFacade_RunTask(const char *name, int workload)
{
    int throughput;
    int cost_ms;

    if (g_initialized == 0) {
        PowerFacade_Init();
    }

    if (name == 0) {
        name = "Unnamed Task";
    }
    if (workload < 0) {
        workload = 0;
    }

    if (g_status.mode == POWER_MODE_OFF) {
        printf("[Task] %s blocked: power mode is OFF\n", name);
        return;
    }

    if (g_status.mode == POWER_MODE_PERFORMANCE) {
        throughput = 400;
    } else if (g_status.mode == POWER_MODE_LOW_POWER) {
        throughput = 80;
    } else {
        throughput = 200;
    }

    cost_ms = (workload * 1000 + throughput - 1) / throughput;

    printf("[Task] %s running in %s (workload=%d)\n", name, mode_to_string(g_status.mode), workload);
    printf("[Task] %s done in %d ms\n", name, cost_ms);
}

void PowerFacade_Shutdown(void)
{
    if (g_initialized == 0) {
        return;
    }

    if (g_status.mode != POWER_MODE_OFF) {
        PowerFacade_SetMode(POWER_MODE_OFF);
    }
    puts("[Facade] Shutdown complete");
}
