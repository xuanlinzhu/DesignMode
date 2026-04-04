#include "instrument_states.h"

extern int printf(const char *format, ...);

static void idle_on_enter(state_t *self, state_context_t *ctx)
{
    (void)ctx;
    printf("[State] enter %s\n", self->name);
}

static void idle_on_exit(state_t *self, state_context_t *ctx)
{
    (void)ctx;
    printf("[State] exit %s\n", self->name);
}

static void idle_handle_event(state_t *self, state_context_t *ctx, event_t event)
{
    (void)self;

    switch (event)
    {
        case EVENT_START:
            printf("[IdleState] receive START, instrument begin running\n");
            state_machine_transition(ctx, &running_state);
            break;
        case EVENT_ENTER_DEBUG:
            printf("[IdleState] receive ENTER_DEBUG, go to debug mode\n");
            state_machine_transition(ctx, &debug_state);
            break;
        case EVENT_ALARM_ON:
            printf("[IdleState] receive ALARM_ON, go to alarm state\n");
            state_machine_transition(ctx, &alarm_state);
            break;
        case EVENT_STOP:
            printf("[IdleState] already idle, ignore STOP\n");
            break;
        case EVENT_ALARM_CLEAR:
            printf("[IdleState] no alarm, ignore ALARM_CLEAR\n");
            break;
        case EVENT_EXIT_DEBUG:
            printf("[IdleState] not in debug, ignore EXIT_DEBUG\n");
            break;
        default:
            break;
    }
}

state_t idle_state = {
    "IDLE",
    idle_on_enter,
    idle_on_exit,
    idle_handle_event
};
