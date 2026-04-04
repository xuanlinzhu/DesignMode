#include "mediator.h"

#include <stdio.h>

static const char *event_to_string(event_t event)
{
    switch (event)
    {
        case EVENT_ALARM_ON:
            return "ALARM_ON";
        case EVENT_ALARM_OFF:
            return "ALARM_OFF";
        case EVENT_MANUAL_ALARM_ON:
            return "MANUAL_ALARM_ON";
        case EVENT_ENTER_DEBUG:
            return "ENTER_DEBUG";
        case EVENT_EXIT_DEBUG:
            return "EXIT_DEBUG";
        case EVENT_START_RUN:
            return "START_RUN";
        case EVENT_STOP_RUN:
            return "STOP_RUN";
        default:
            return "UNKNOWN";
    }
}

static void handle_alarm_on_common(instrument_mediator_t *self)
{
    self->alarm_active = 1;

    if (self->running == 1) {
        printf("[Mediator] stop running due to alarm\n");
        run_module_stop(self->run);
        self->running = 0;
    }

    display_show_alarm(self->display);
    buzzer_on(self->buzzer);
    led_set_red_blink(self->led);
}

void mediator_init(instrument_mediator_t *self)
{
    if (self == 0) {
        return;
    }

    self->alarm_active = 0;
    self->debug_mode = 0;
    self->running = 0;

    self->alarm = 0;
    self->debug = 0;
    self->run = 0;
    self->display = 0;
    self->buzzer = 0;
    self->led = 0;
}

void mediator_bind_modules(
    instrument_mediator_t *self,
    alarm_module_t *alarm,
    debug_module_t *debug,
    run_module_t *run,
    display_module_t *display,
    buzzer_module_t *buzzer,
    led_module_t *led)
{
    if (self == 0) {
        return;
    }

    self->alarm = alarm;
    self->debug = debug;
    self->run = run;
    self->display = display;
    self->buzzer = buzzer;
    self->led = led;
}

void mediator_notify(instrument_mediator_t *self, void *sender, event_t event)
{
    (void)sender;

    if (self == 0) {
        return;
    }

    printf("[Mediator] event: %s\n", event_to_string(event));

    switch (event)
    {
        case EVENT_ALARM_ON:
            handle_alarm_on_common(self);
            break;

        case EVENT_MANUAL_ALARM_ON:
            printf("[Mediator] handle manual alarm as normal alarm flow\n");
            handle_alarm_on_common(self);
            break;

        case EVENT_ALARM_OFF:
            self->alarm_active = 0;
            buzzer_off(self->buzzer);
            if (self->debug_mode == 1) {
                display_show_debug(self->display);
                led_set_yellow_on(self->led);
            } else if (self->running == 1) {
                display_show_run(self->display);
                led_set_green_on(self->led);
            } else {
                display_show_main(self->display);
                led_set_default(self->led);
            }
            break;

        case EVENT_ENTER_DEBUG:
            self->debug_mode = 1;
            if (self->running == 1) {
                printf("[Mediator] stop running due to enter debug\n");
                run_module_stop(self->run);
                self->running = 0;
            }

            if (self->alarm_active == 1) {
                printf("[Mediator] alarm active, keep alarm page/RED LED, only update debug_mode\n");
            } else {
                display_show_debug(self->display);
                led_set_yellow_on(self->led);
            }
            break;

        case EVENT_EXIT_DEBUG:
            self->debug_mode = 0;
            if (self->alarm_active == 1) {
                display_show_alarm(self->display);
                led_set_red_blink(self->led);
            } else if (self->running == 1) {
                display_show_run(self->display);
                led_set_green_on(self->led);
            } else {
                display_show_main(self->display);
                led_set_default(self->led);
            }
            break;

        case EVENT_START_RUN:
            if (self->alarm_active == 1) {
                printf("[Mediator] deny start: alarm active\n");
                break;
            }
            if (self->debug_mode == 1) {
                printf("[Mediator] deny start: debug mode active\n");
                break;
            }

            run_module_start(self->run);
            self->running = 1;
            display_show_run(self->display);
            led_set_green_on(self->led);
            break;

        case EVENT_STOP_RUN:
            if (self->running == 1) {
                run_module_stop(self->run);
            }
            self->running = 0;

            if (self->alarm_active == 1) {
                display_show_alarm(self->display);
                led_set_red_blink(self->led);
            } else if (self->debug_mode == 1) {
                display_show_debug(self->display);
                led_set_yellow_on(self->led);
            } else {
                display_show_main(self->display);
                led_set_default(self->led);
            }
            break;

        default:
            break;
    }
}

void mediator_dump_state(const instrument_mediator_t *self)
{
    if (self == 0) {
        return;
    }

    printf("[MediatorState] alarm=%d debug=%d running=%d\n",
           self->alarm_active,
           self->debug_mode,
           self->running);
}
