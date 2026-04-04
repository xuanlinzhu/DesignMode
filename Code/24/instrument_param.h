#ifndef INSTRUMENT_PARAM_H
#define INSTRUMENT_PARAM_H

#include "param_memento.h"

/* Originator：当前仪器参数对象。 */
typedef struct
{
    int gain;
    int offset;
    int threshold;
    int mode;
} instrument_param_t;

void instrument_param_init(instrument_param_t *self, int gain, int offset, int threshold, int mode);
void instrument_param_print(const instrument_param_t *self);

param_memento_t instrument_param_create_memento(const instrument_param_t *self);
void instrument_param_restore_memento(instrument_param_t *self, const param_memento_t *memento);

void instrument_param_set_gain(instrument_param_t *self, int gain);
void instrument_param_set_offset(instrument_param_t *self, int offset);
void instrument_param_set_threshold(instrument_param_t *self, int threshold);
void instrument_param_set_mode(instrument_param_t *self, int mode);

#endif
