#include "element.h"
#include "visitor.h"

static void motor_accept(element_t *base, visitor_t *visitor)
{
    if (base == 0 || visitor == 0 || visitor->visit_motor == 0) {
        return;
    }

    visitor->visit_motor(visitor, (motor_t *)base);
}

void motor_init(motor_t *self, int speed)
{
    if (self == 0) {
        return;
    }

    self->base.accept = motor_accept;
    self->speed = speed;
}
