#ifndef ELEMENT_H
#define ELEMENT_H

typedef struct visitor visitor_t;

typedef struct element
{
    void (*accept)(struct element *self, visitor_t *visitor);
} element_t;

typedef struct temp_sensor
{
    element_t base;
    int temperature;
} temp_sensor_t;

typedef struct pressure_sensor
{
    element_t base;
    int pressure;
} pressure_sensor_t;

typedef struct motor
{
    element_t base;
    int speed;
} motor_t;

void temp_sensor_init(temp_sensor_t *self, int temperature);
void pressure_sensor_init(pressure_sensor_t *self, int pressure);
void motor_init(motor_t *self, int speed);

#endif
