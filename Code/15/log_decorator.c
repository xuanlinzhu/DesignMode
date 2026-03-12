#include "log_decorator.h"

extern int snprintf(char *str, size_t size, const char *format, ...);

/*
 * 标签插入规则（供三个具体装饰器复用）：
 * - message 前缀若已经是若干个 "[xxx] "，先扫描出这段前缀
 * - 新标签插入在该前缀之后
 * 这样可避免后加的装饰器把前面的标签“打散”到中间。
 */
int log_decorator_insert_tag(const char *tag, const char *message, char *out, size_t out_cap)
{
    size_t pos;
    size_t i;
    int n;

    if (tag == NULL || message == NULL || out == NULL || out_cap == 0U) {
        return -1;
    }

    pos = 0U;
    /* 扫描 message 开头连续的 [token] 段 */
    while (message[pos] == '[') {
        i = pos + 1U;
        while (message[i] != '\0' && message[i] != ']') {
            i += 1U;
        }
        /* 出现不完整标签（缺少 ]）时停止扫描，按普通文本处理 */
        if (message[i] != ']') {
            break;
        }

        pos = i + 1U;
        /* 跳过 ] 后可能存在的一个空格 */
        if (message[pos] == ' ') {
            pos += 1U;
        }
    }

    /*
     * 以“前缀 + [新标签] + 空格 + 余下正文”形式重建。
     * %.*s 用 pos 控制复制长度，避免手工拷贝出错。
     */
    n = snprintf(out, out_cap, "%.*s[%s] %s", (int)pos, message, tag, message + pos);
    if (n < 0 || (size_t)n >= out_cap) {
        return -1;
    }

    return 0;
}
