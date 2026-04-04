#ifndef VISITOR_H
#define VISITOR_H

typedef struct temp_sensor temp_sensor_t;
typedef struct pressure_sensor pressure_sensor_t;
typedef struct motor motor_t;

typedef struct visitor
{
    void (*visit_temp)(struct visitor *self, temp_sensor_t *elem);
    void (*visit_pressure)(struct visitor *self, pressure_sensor_t *elem);
    void (*visit_motor)(struct visitor *self, motor_t *elem);
} visitor_t;

typedef struct
{
    visitor_t base;
} info_visitor_t;

typedef struct
{
    visitor_t base;
} selftest_visitor_t;

void info_visitor_init(info_visitor_t *self);
void selftest_visitor_init(selftest_visitor_t *self);

#endif
