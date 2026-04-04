#include "state_machine.h"

extern int printf(const char *format, ...);

const char *event_to_string(event_t event)
{
    switch (event)
    {
        case EVENT_START:
            return "START";
        case EVENT_STOP:
            return "STOP";
        case EVENT_ALARM_ON:
            return "ALARM_ON";
        case EVENT_ALARM_CLEAR:
            return "ALARM_CLEAR";
        case EVENT_ENTER_DEBUG:
            return "ENTER_DEBUG";
        case EVENT_EXIT_DEBUG:
            return "EXIT_DEBUG";
        default:
            return "UNKNOWN";
    }
}

void state_machine_init(state_context_t *ctx, state_t *init_state)
{
    if (ctx == 0 || init_state == 0) {
        return;
    }

    ctx->current_state = init_state;
    printf("[FSM] init -> %s\n", ctx->current_state->name);

    if (ctx->current_state->on_enter != 0) {
        ctx->current_state->on_enter(ctx->current_state, ctx);
    }
}

void state_machine_transition(state_context_t *ctx, state_t *target_state)
{
    state_t *old_state;

    if (ctx == 0 || ctx->current_state == 0 || target_state == 0) {
        return;
    }

    old_state = ctx->current_state;
    printf("[FSM] transition: %s -> %s\n", old_state->name, target_state->name);

    if (old_state->on_exit != 0) {
        old_state->on_exit(old_state, ctx);
    }

    ctx->current_state = target_state;

    if (ctx->current_state->on_enter != 0) {
        ctx->current_state->on_enter(ctx->current_state, ctx);
    }
}

void state_machine_dispatch(state_context_t *ctx, event_t event)
{
    if (ctx == 0 || ctx->current_state == 0 || ctx->current_state->handle_event == 0) {
        return;
    }

    printf("[FSM] event %s in state %s\n", event_to_string(event), ctx->current_state->name);
    ctx->current_state->handle_event(ctx->current_state, ctx, event);
}
