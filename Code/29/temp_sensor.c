#include "element.h"
#include "visitor.h"

static void temp_sensor_accept(element_t *base, visitor_t *visitor)
{
    if (base == 0 || visitor == 0 || visitor->visit_temp == 0) {
        return;
    }

    /* 双分派：元素把自己作为 temp_sensor_t 交给 visitor。 */
    visitor->visit_temp(visitor, (temp_sensor_t *)base);
}

void temp_sensor_init(temp_sensor_t *self, int temperature)
{
    if (self == 0) {
        return;
    }

    self->base.accept = temp_sensor_accept;
    self->temperature = temperature;
}
