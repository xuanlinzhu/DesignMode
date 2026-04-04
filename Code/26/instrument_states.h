#ifndef INSTRUMENT_STATES_H
#define INSTRUMENT_STATES_H

#include "state_machine.h"

/*
 * 四个状态对象在各自 .c 文件中定义。
 * 客户端与状态机框架只通过 state_t * 指针交互。
 */
extern state_t idle_state;
extern state_t running_state;
extern state_t alarm_state;
extern state_t debug_state;

#endif
