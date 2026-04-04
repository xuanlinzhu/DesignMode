#include "instrument_states.h"

extern int printf(const char *format, ...);

static void debug_on_enter(state_t *self, state_context_t *ctx)
{
    (void)ctx;
    printf("[State] enter %s\n", self->name);
}

static void debug_on_exit(state_t *self, state_context_t *ctx)
{
    (void)ctx;
    printf("[State] exit %s\n", self->name);
}

static void debug_handle_event(state_t *self, state_context_t *ctx, event_t event)
{
    (void)self;

    switch (event)
    {
        case EVENT_EXIT_DEBUG:
            printf("[DebugState] exit debug mode, back to idle\n");
            state_machine_transition(ctx, &idle_state);
            break;
        case EVENT_ALARM_ON:
            printf("[DebugState] alarm triggered in debug mode\n");
            state_machine_transition(ctx, &alarm_state);
            break;
        case EVENT_START:
            printf("[DebugState] cannot start while in debug mode\n");
            break;
        case EVENT_ENTER_DEBUG:
            printf("[DebugState] already in debug mode, ignore ENTER_DEBUG\n");
            break;
        case EVENT_STOP:
            printf("[DebugState] not running in debug mode, ignore STOP\n");
            break;
        case EVENT_ALARM_CLEAR:
            printf("[DebugState] no alarm clear action in debug state, ignore ALARM_CLEAR\n");
            break;
        default:
            break;
    }
}

state_t debug_state = {
    "DEBUG",
    debug_on_enter,
    debug_on_exit,
    debug_handle_event
};
