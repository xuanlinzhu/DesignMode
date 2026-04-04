#include "element.h"
#include "visitor.h"

extern int printf(const char *format, ...);

static void info_visit_temp(visitor_t *base, temp_sensor_t *elem)
{
    (void)base;
    printf("[Info] TempSensor: %d\n", elem->temperature);
}

static void info_visit_pressure(visitor_t *base, pressure_sensor_t *elem)
{
    (void)base;
    printf("[Info] PressureSensor: %d\n", elem->pressure);
}

static void info_visit_motor(visitor_t *base, motor_t *elem)
{
    (void)base;
    printf("[Info] Motor speed: %d\n", elem->speed);
}

void info_visitor_init(info_visitor_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.visit_temp = info_visit_temp;
    self->base.visit_pressure = info_visit_pressure;
    self->base.visit_motor = info_visit_motor;
}
