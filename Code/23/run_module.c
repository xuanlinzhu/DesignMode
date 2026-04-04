#include "modules.h"

#include <stdio.h>

#include "mediator.h"

void run_module_init(run_module_t *self, instrument_mediator_t *mediator)
{
    if (self == 0) {
        return;
    }
    self->mediator = mediator;
    self->running = 0;
}

/* 对外接口：只发事件，不直接修改其他模块。 */
void run_module_request_start(run_module_t *self)
{
    if (self == 0 || self->mediator == 0) {
        return;
    }
    printf("[RunModule] request start\n");
    mediator_notify(self->mediator, self, EVENT_START_RUN);
}

void run_module_request_stop(run_module_t *self)
{
    if (self == 0 || self->mediator == 0) {
        return;
    }
    printf("[RunModule] request stop\n");
    mediator_notify(self->mediator, self, EVENT_STOP_RUN);
}

/* 对内接口：由中介者调用，真正改变运行模块内部状态。 */
void run_module_start(run_module_t *self)
{
    if (self == 0) {
        return;
    }
    self->running = 1;
    printf("[RunModule] start\n");
}

void run_module_stop(run_module_t *self)
{
    if (self == 0) {
        return;
    }
    self->running = 0;
    printf("[RunModule] stop\n");
}
