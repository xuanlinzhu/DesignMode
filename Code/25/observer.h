#ifndef OBSERVER_H
#define OBSERVER_H

/*
 * 观察者抽象接口。
 *
 * 约束：
 * 1. Subject 只能通过 update 统一通知入口调用观察者。
 * 2. Subject 不能感知具体观察者类型，也不能调用具体业务函数。
 */
typedef struct observer
{
    void (*update)(struct observer *self, int value);
} observer_t;

#endif
