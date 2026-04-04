#ifndef MODULES_H
#define MODULES_H

typedef struct instrument_mediator instrument_mediator_t;

typedef struct
{
    instrument_mediator_t *mediator;
} alarm_module_t;

typedef struct
{
    instrument_mediator_t *mediator;
} debug_module_t;

typedef struct
{
    instrument_mediator_t *mediator;
    int running;
} run_module_t;

typedef struct
{
    instrument_mediator_t *mediator;
} display_module_t;

typedef struct
{
    instrument_mediator_t *mediator;
    int on;
} buzzer_module_t;

typedef struct
{
    instrument_mediator_t *mediator;
} led_module_t;

/* AlarmModule */
void alarm_module_init(alarm_module_t *self, instrument_mediator_t *mediator);
void alarm_module_trigger(alarm_module_t *self);
void alarm_module_clear(alarm_module_t *self);
void alarm_module_manual_trigger(alarm_module_t *self);

/* DebugModule */
void debug_module_init(debug_module_t *self, instrument_mediator_t *mediator);
void debug_module_enter(debug_module_t *self);
void debug_module_exit(debug_module_t *self);

/* RunModule */
void run_module_init(run_module_t *self, instrument_mediator_t *mediator);
void run_module_request_start(run_module_t *self);
void run_module_request_stop(run_module_t *self);
void run_module_start(run_module_t *self);
void run_module_stop(run_module_t *self);

/* DisplayModule */
void display_module_init(display_module_t *self, instrument_mediator_t *mediator);
void display_show_main(display_module_t *self);
void display_show_alarm(display_module_t *self);
void display_show_debug(display_module_t *self);
void display_show_run(display_module_t *self);

/* BuzzerModule */
void buzzer_module_init(buzzer_module_t *self, instrument_mediator_t *mediator);
void buzzer_on(buzzer_module_t *self);
void buzzer_off(buzzer_module_t *self);

/* LedModule */
void led_module_init(led_module_t *self, instrument_mediator_t *mediator);
void led_set_default(led_module_t *self);
void led_set_red_blink(led_module_t *self);
void led_set_yellow_on(led_module_t *self);
void led_set_green_on(led_module_t *self);

#endif
