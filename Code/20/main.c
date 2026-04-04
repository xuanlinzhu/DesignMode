#include <stdio.h>
#include <string.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "command_pool.h"
#include "command_queue.h"

/*
 * 通信线程上下文：仅负责“接收报文 -> 解析 -> 创建命令 -> 入队”。
 * 注意这里不持有执行逻辑，不直接调用 receiver 动作函数。
 */
typedef struct
{
    const char **msgs;
    int msg_count;
    int next_index;

    led_receiver_t *led;
    motor_receiver_t *motor;
    config_receiver_t *config;
} comm_context_t;

/*
 * 在 Windows 控制台下显式切换到 UTF-8 输出。
 *
 * Code/20 的演示日志包含中文；如果不主动设置控制台代码页，
 * 在部分 WinPC 环境中会把 UTF-8 字符串按本地旧代码页解释，导致乱码。
 */
static void init_console_utf8(void)
{
    (void)setlocale(LC_ALL, ".UTF-8");

#ifdef _WIN32
    (void)SetConsoleOutputCP(65001u);
    (void)SetConsoleCP(65001u);
#endif
}

/*
 * 执行线程一步：
 * 只做 pop + execute + destroy。
 * 没有大 switch，完全依赖命令多态行为。
 */
static void worker_thread_step(command_queue_t *queue)
{
    command_t *cmd;

    if (queue == 0) {
        return;
    }

    cmd = command_queue_pop(queue);
    if (cmd == 0) {
        printf("[Worker] queue empty\n");
        return;
    }

    printf("[Worker] pop command: %s\n", cmd->name);
    cmd->execute(cmd);
    cmd->destroy(cmd);
}

/*
 * 报文解析 + 命令创建。
 * 这里允许做轻量字符串判断，但仅用于“创建哪种命令对象”，
 * 不允许直接触发接收者动作。
 */
static command_t *create_command_from_msg(const char *msg, comm_context_t *ctx)
{
    int value = 0;

    if (msg == 0 || ctx == 0) {
        return 0;
    }

    if (strcmp(msg, "LED_ON") == 0) {
        printf("[Comm] create: LedOnCommand\n");
        return led_on_command_create(ctx->led);
    }

    if (strcmp(msg, "LED_OFF") == 0) {
        printf("[Comm] create: LedOffCommand\n");
        return led_off_command_create(ctx->led);
    }

    if (sscanf(msg, "MOTOR_SET %d", &value) == 1) {
        printf("[Comm] create: MotorSetSpeedCommand(speed=%d)\n", value);
        return motor_set_speed_command_create(ctx->motor, value);
    }

    if (sscanf(msg, "SAVE_CONFIG %d", &value) == 1) {
        printf("[Comm] create: SaveConfigCommand(config_id=%d)\n", value);
        return save_config_command_create(ctx->config, value);
    }

    printf("[Comm] error: unsupported message '%s'\n", msg);
    return 0;
}

/*
 * 通信线程一步：处理一条报文并立刻返回。
 * 即使后续存在耗时命令，也不会在此处阻塞。
 */
static void comm_thread_step(comm_context_t *ctx, command_queue_t *queue)
{
    const char *msg;
    command_t *cmd;

    if (ctx == 0 || queue == 0) {
        return;
    }

    if (ctx->next_index >= ctx->msg_count) {
        printf("[Comm] no more message\n");
        return;
    }

    msg = ctx->msgs[ctx->next_index++];
    printf("[Comm] recv: %s\n", msg);

    cmd = create_command_from_msg(msg, ctx);
    if (cmd == 0) {
        printf("[Comm] skip invalid message\n");
        return;
    }

    if (!command_queue_push(queue, cmd)) {
        printf("[Comm] queue full, drop command: %s\n", cmd->name);
        cmd->destroy(cmd);
        return;
    }

    printf("[Comm] push queue success\n");
}

int main(void)
{
    /* 设计说明要求的固定演示序列。 */
    static const char *msgs[] = {
        "LED_ON",
        "MOTOR_SET 120",
        "SAVE_CONFIG 3",
        "LED_OFF"
    };

    led_receiver_t led;
    motor_receiver_t motor;
    config_receiver_t config;

    command_queue_t queue;
    comm_context_t comm;
    int tick;

    init_console_utf8();

    led_receiver_init(&led);
    motor_receiver_init(&motor);
    config_receiver_init(&config);

    command_pool_init();
    command_queue_init(&queue);

    comm.msgs = msgs;
    comm.msg_count = (int)(sizeof(msgs) / sizeof(msgs[0]));
    comm.next_index = 0;
    comm.led = &led;
    comm.motor = &motor;
    comm.config = &config;

    printf("==============================\n");
    printf("[Demo] Command 模式：通信侧与执行侧解耦\n");
    printf("==============================\n");

    /*
     * 调度策略：先连续推动通信侧两次，再执行侧一次，重复。
     * 这样能直观看到 SAVE_CONFIG 入队后，通信侧仍继续处理 LED_OFF，
     * 证明耗时动作不会阻塞通信线程。
     */
    tick = 0;
    while (comm.next_index < comm.msg_count || !command_queue_is_empty(&queue)) {
        printf("\n--- scheduler tick %d ---\n", tick++);

        if (comm.next_index < comm.msg_count) {
            comm_thread_step(&comm, &queue);
        }
        if (comm.next_index < comm.msg_count) {
            comm_thread_step(&comm, &queue);
        }
        worker_thread_step(&queue);
    }

    printf("\n==============================\n");
    printf("[Demo] done\n");
    return 0;
}
