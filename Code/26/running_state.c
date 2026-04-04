#include "instrument_states.h"

extern int printf(const char *format, ...);

static void running_on_enter(state_t *self, state_context_t *ctx)
{
    (void)ctx;
    printf("[State] enter %s\n", self->name);
}

static void running_on_exit(state_t *self, state_context_t *ctx)
{
    (void)ctx;
    printf("[State] exit %s\n", self->name);
}

static void running_handle_event(state_t *self, state_context_t *ctx, event_t event)
{
    (void)self;

    switch (event)
    {
        case EVENT_STOP:
            printf("[RunningState] receive STOP, stop running\n");
            state_machine_transition(ctx, &idle_state);
            break;
        case EVENT_ALARM_ON:
            printf("[RunningState] alarm triggered while running\n");
            state_machine_transition(ctx, &alarm_state);
            break;
        case EVENT_START:
            printf("[RunningState] already running, ignore START\n");
            break;
        case EVENT_ENTER_DEBUG:
            printf("[RunningState] cannot enter debug while running\n");
            break;
        case EVENT_ALARM_CLEAR:
            printf("[RunningState] no alarm in running state, ignore ALARM_CLEAR\n");
            break;
        case EVENT_EXIT_DEBUG:
            printf("[RunningState] not in debug mode, ignore EXIT_DEBUG\n");
            break;
        default:
            break;
    }
}

state_t running_state = {
    "RUNNING",
    running_on_enter,
    running_on_exit,
    running_handle_event
};
