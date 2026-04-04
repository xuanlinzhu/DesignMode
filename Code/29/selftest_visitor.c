#include "element.h"
#include "visitor.h"

extern int printf(const char *format, ...);

static void selftest_visit_temp(visitor_t *base, temp_sensor_t *elem)
{
    (void)base;
    if (elem->temperature >= -40 && elem->temperature <= 125) {
        printf("[SelfTest] TempSensor OK (value=%d)\n", elem->temperature);
    }
    else {
        printf("[SelfTest] TempSensor FAIL (value=%d)\n", elem->temperature);
    }
}

static void selftest_visit_pressure(visitor_t *base, pressure_sensor_t *elem)
{
    (void)base;
    if (elem->pressure >= 0 && elem->pressure <= 300) {
        printf("[SelfTest] PressureSensor OK (value=%d)\n", elem->pressure);
    }
    else {
        printf("[SelfTest] PressureSensor FAIL (value=%d)\n", elem->pressure);
    }
}

static void selftest_visit_motor(visitor_t *base, motor_t *elem)
{
    (void)base;
    if (elem->speed >= 0 && elem->speed <= 3000) {
        printf("[SelfTest] Motor OK (speed=%d)\n", elem->speed);
    }
    else {
        printf("[SelfTest] Motor FAIL (speed=%d)\n", elem->speed);
    }
}

void selftest_visitor_init(selftest_visitor_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.visit_temp = selftest_visit_temp;
    self->base.visit_pressure = selftest_visit_pressure;
    self->base.visit_motor = selftest_visit_motor;
}
