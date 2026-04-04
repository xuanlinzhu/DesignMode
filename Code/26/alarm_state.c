#include "instrument_states.h"

extern int printf(const char *format, ...);

static void alarm_on_enter(state_t *self, state_context_t *ctx)
{
    (void)ctx;
    printf("[State] enter %s\n", self->name);
}

static void alarm_on_exit(state_t *self, state_context_t *ctx)
{
    (void)ctx;
    printf("[State] exit %s\n", self->name);
}

static void alarm_handle_event(state_t *self, state_context_t *ctx, event_t event)
{
    (void)self;

    switch (event)
    {
        case EVENT_ALARM_CLEAR:
            printf("[AlarmState] alarm cleared, back to idle\n");
            state_machine_transition(ctx, &idle_state);
            break;
        case EVENT_START:
            printf("[AlarmState] cannot start while alarm active\n");
            break;
        case EVENT_STOP:
            printf("[AlarmState] already stopped by alarm, ignore STOP\n");
            break;
        case EVENT_ENTER_DEBUG:
            printf("[AlarmState] cannot enter debug while alarm active\n");
            break;
        case EVENT_ALARM_ON:
            printf("[AlarmState] alarm already active, ignore ALARM_ON\n");
            break;
        case EVENT_EXIT_DEBUG:
            printf("[AlarmState] not in debug mode, ignore EXIT_DEBUG\n");
            break;
        default:
            break;
    }
}

state_t alarm_state = {
    "ALARM",
    alarm_on_enter,
    alarm_on_exit,
    alarm_handle_event
};
