#include "log_writer.h"

extern int printf(const char *format, ...);
extern int puts(const char *str);

/*
 * 辅助函数：
 * - 若 next 创建成功，返回新外层对象
 * - 若 next 创建失败，销毁 current（防止泄漏）并返回 NULL
 */
static LogWriter *replace_or_destroy(LogWriter *current, LogWriter *next)
{
    if (next != 0) {
        return next;
    }

    if (current != 0 && current->destroy != 0) {
        current->destroy(current);
    }
    return 0;
}

int main(void)
{
    LogWriter *chain1;
    LogWriter *chain2;

    /*
     * 示例 1：Timestamp(Level(Module(FileLogWriter)))
     * 构造顺序从内到外：File -> Module -> Level -> Timestamp
     */
    chain1 = file_log_writer_create("app.log");
    if (chain1 == 0) {
        puts("create FileLogWriter failed");
        return 1;
    }

    chain1 = replace_or_destroy(chain1, module_decorator_create(chain1, "Sensor"));
    if (chain1 == 0) {
        puts("create ModuleDecorator failed");
        return 1;
    }
    chain1 = replace_or_destroy(chain1, level_decorator_create(chain1, LOG_LEVEL_ERROR));
    if (chain1 == 0) {
        puts("create LevelDecorator failed");
        return 1;
    }
    chain1 = replace_or_destroy(chain1, timestamp_decorator_create(chain1));
    if (chain1 == 0) {
        puts("create TimestampDecorator failed");
        return 1;
    }

    /* 通过最外层对象写入，调用会沿装饰链逐层转发 */
    if (chain1->write(chain1, "read failed") != 0) {
        puts("write failed: read failed");
        chain1->destroy(chain1);
        return 1;
    }
    if (chain1->write(chain1, "init timeout") != 0) {
        puts("write failed: init timeout");
        chain1->destroy(chain1);
        return 1;
    }
    /* 从最外层 destroy，一次释放整条链 */
    chain1->destroy(chain1);

    /*
     * 示例 2：Level(Timestamp(FileLogWriter))
     * 与示例 1 调整顺序，用于演示装饰器可自由组合。
     */
    chain2 = file_log_writer_create("app.log");
    if (chain2 == 0) {
        puts("create FileLogWriter for chain2 failed");
        return 1;
    }

    chain2 = replace_or_destroy(chain2, timestamp_decorator_create(chain2));
    if (chain2 == 0) {
        puts("create TimestampDecorator for chain2 failed");
        return 1;
    }
    chain2 = replace_or_destroy(chain2, level_decorator_create(chain2, LOG_LEVEL_INFO));
    if (chain2 == 0) {
        puts("create LevelDecorator for chain2 failed");
        return 1;
    }

    if (chain2->write(chain2, "logger started") != 0) {
        puts("write failed: logger started");
        chain2->destroy(chain2);
        return 1;
    }
    if (chain2->write(chain2, "background task running") != 0) {
        puts("write failed: background task running");
        chain2->destroy(chain2);
        return 1;
    }
    chain2->destroy(chain2);

    /* 教学输出：明确模式角色与边界 */
    puts("Decorator demo done. Check app.log");
    puts("Component: LogWriter interface");
    puts("ConcreteComponent: FileLogWriter");
    puts("Decorator: LogWriterDecorator (has wrapped LogWriter*)");
    puts("ConcreteDecorator: Timestamp/Level/Module decorators");
    puts("Why Decorator: each enhancement is an object wrapper and can be stacked dynamically.");
    puts("Not Builder/Strategy: we are not assembling a product step-by-step, and not switching one algorithm.");
    return 0;
}
