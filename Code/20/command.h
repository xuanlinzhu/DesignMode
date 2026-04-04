#ifndef COMMAND_H
#define COMMAND_H

/*
 * 命令抽象接口：
 * 1. execute 负责触发具体业务动作
 * 2. destroy 负责释放命令对象占用资源（本示例回收到静态池）
 *
 * 通信侧只创建/入队 command_t*，执行侧只调用 execute/destroy，
 * 不需要知道具体命令类型。
 */
typedef struct command
{
    void (*execute)(struct command *self);
    void (*destroy)(struct command *self);
    const char *name;
} command_t;

#endif
