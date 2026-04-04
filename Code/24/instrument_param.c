#include "instrument_param.h"

#include <stdio.h>

void instrument_param_init(instrument_param_t *self, int gain, int offset, int threshold, int mode)
{
    if (self == 0) {
        return;
    }

    self->gain = gain;
    self->offset = offset;
    self->threshold = threshold;
    self->mode = mode;

    printf("[Param] init: gain=%d offset=%d threshold=%d mode=%d\n",
           self->gain,
           self->offset,
           self->threshold,
           self->mode);
}

void instrument_param_print(const instrument_param_t *self)
{
    if (self == 0) {
        return;
    }

    printf("[Param] current: gain=%d offset=%d threshold=%d mode=%d\n",
           self->gain,
           self->offset,
           self->threshold,
           self->mode);
}

param_memento_t instrument_param_create_memento(const instrument_param_t *self)
{
    param_memento_t memento;

    memento.gain = 0;
    memento.offset = 0;
    memento.threshold = 0;
    memento.mode = 0;

    if (self == 0) {
        return memento;
    }

    memento.gain = self->gain;
    memento.offset = self->offset;
    memento.threshold = self->threshold;
    memento.mode = self->mode;
    return memento;
}

void instrument_param_restore_memento(instrument_param_t *self, const param_memento_t *memento)
{
    if (self == 0 || memento == 0) {
        return;
    }

    self->gain = memento->gain;
    self->offset = memento->offset;
    self->threshold = memento->threshold;
    self->mode = memento->mode;
}

void instrument_param_set_gain(instrument_param_t *self, int gain)
{
    if (self == 0) {
        return;
    }
    self->gain = gain;
    printf("[Param] set gain=%d\n", gain);
}

void instrument_param_set_offset(instrument_param_t *self, int offset)
{
    if (self == 0) {
        return;
    }
    self->offset = offset;
    printf("[Param] set offset=%d\n", offset);
}

void instrument_param_set_threshold(instrument_param_t *self, int threshold)
{
    if (self == 0) {
        return;
    }
    self->threshold = threshold;
    printf("[Param] set threshold=%d\n", threshold);
}

void instrument_param_set_mode(instrument_param_t *self, int mode)
{
    if (self == 0) {
        return;
    }
    self->mode = mode;
    printf("[Param] set mode=%d\n", mode);
}
