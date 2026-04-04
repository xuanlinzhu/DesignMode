#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "modules.h"

typedef enum
{
    EVENT_ALARM_ON = 0,
    EVENT_ALARM_OFF,
    EVENT_MANUAL_ALARM_ON,
    EVENT_ENTER_DEBUG,
    EVENT_EXIT_DEBUG,
    EVENT_START_RUN,
    EVENT_STOP_RUN
} event_t;

/*
 * 中介者对象：
 * - 维护系统核心状态
 * - 统一协调所有模块联动
 */
typedef struct instrument_mediator
{
    int alarm_active;
    int debug_mode;
    int running;

    alarm_module_t *alarm;
    debug_module_t *debug;
    run_module_t *run;
    display_module_t *display;
    buzzer_module_t *buzzer;
    led_module_t *led;
} instrument_mediator_t;

void mediator_init(instrument_mediator_t *self);
void mediator_bind_modules(
    instrument_mediator_t *self,
    alarm_module_t *alarm,
    debug_module_t *debug,
    run_module_t *run,
    display_module_t *display,
    buzzer_module_t *buzzer,
    led_module_t *led);

void mediator_notify(instrument_mediator_t *self, void *sender, event_t event);
void mediator_dump_state(const instrument_mediator_t *self);

#endif
