#include <stdio.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "context.h"
#include "parser.h"

static void init_console_utf8(void)
{
    (void)setlocale(LC_ALL, ".UTF-8");

#ifdef _WIN32
    (void)SetConsoleOutputCP(65001u);
    (void)SetConsoleCP(65001u);
#endif
}

int main(void)
{
    context_t ctx;

    init_console_utf8();

    /*
     * 按设计说明固定脚本执行，不实现交互式 shell。
     * 场景覆盖：创建、修改、打印、删除、表达式、删除后失败。
     */
    static const char *scripts[] = {
        "a = 10",
        "b = 5",
        "PRINT a",
        "a + b",
        "a = a + 3",
        "PRINT a",
        "b = a - 2",
        "PRINT b",
        "DEL a",
        "PRINT a",
        "10 + 2",
        "b - 1"
    };
    int i;

    context_init(&ctx);

    printf("==============================\n");
    printf("[Demo] Interpreter 模式\n");
    printf("==============================\n");

    for (i = 0; i < (int)(sizeof(scripts) / sizeof(scripts[0])); i++) {
        statement_t *stmt;

        printf("\n[Script] %s\n", scripts[i]);

        stmt = parse_statement(scripts[i]);
        if (stmt == 0) {
            printf("[Error] parse failed: '%s'\n", scripts[i]);
            continue;
        }

        if (!stmt->execute(stmt, &ctx)) {
            printf("[Info] statement execution failed but continue\n");
        }
        stmt->destroy(stmt);
    }

    printf("\n==============================\n");
    printf("[Demo] done\n");
    return 0;
}
