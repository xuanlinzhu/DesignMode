#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum
{
    EVENT_START = 0,
    EVENT_STOP,
    EVENT_ALARM_ON,
    EVENT_ALARM_CLEAR,
    EVENT_ENTER_DEBUG,
    EVENT_EXIT_DEBUG
} event_t;

typedef struct state_context state_context_t;
typedef struct state state_t;

struct state
{
    const char *name;
    void (*on_enter)(state_t *self, state_context_t *ctx);
    void (*on_exit)(state_t *self, state_context_t *ctx);
    void (*handle_event)(state_t *self, state_context_t *ctx, event_t event);
};

struct state_context
{
    state_t *current_state;
};

void state_machine_init(state_context_t *ctx, state_t *init_state);
void state_machine_dispatch(state_context_t *ctx, event_t event);
void state_machine_transition(state_context_t *ctx, state_t *target_state);
const char *event_to_string(event_t event);

#endif
