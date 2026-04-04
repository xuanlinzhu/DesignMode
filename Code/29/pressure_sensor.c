#include "element.h"
#include "visitor.h"

static void pressure_sensor_accept(element_t *base, visitor_t *visitor)
{
    if (base == 0 || visitor == 0 || visitor->visit_pressure == 0) {
        return;
    }

    visitor->visit_pressure(visitor, (pressure_sensor_t *)base);
}

void pressure_sensor_init(pressure_sensor_t *self, int pressure)
{
    if (self == 0) {
        return;
    }

    self->base.accept = pressure_sensor_accept;
    self->pressure = pressure;
}
