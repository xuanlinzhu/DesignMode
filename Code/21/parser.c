#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* 去除首尾空白，结果写入 out。 */
static int trim_copy(const char *src, char *out, int out_size)
{
    int start = 0;
    int end;
    int len;
    int i;

    if (src == 0 || out == 0 || out_size <= 1) {
        return 0;
    }

    len = (int)strlen(src);
    end = len - 1;

    while (start < len && isspace((unsigned char)src[start])) {
        start++;
    }
    while (end >= start && isspace((unsigned char)src[end])) {
        end--;
    }

    if (end < start) {
        out[0] = '\0';
        return 1;
    }

    len = end - start + 1;
    if (len >= out_size) {
        return 0;
    }

    for (i = 0; i < len; i++) {
        out[i] = src[start + i];
    }
    out[len] = '\0';
    return 1;
}

static int is_identifier(const char *text)
{
    int i;

    if (text == 0 || text[0] == '\0') {
        return 0;
    }

    if (!(isalpha((unsigned char)text[0]) || text[0] == '_')) {
        return 0;
    }

    for (i = 1; text[i] != '\0'; i++) {
        if (!(isalnum((unsigned char)text[i]) || text[i] == '_')) {
            return 0;
        }
    }
    return 1;
}

static expression_t *parse_value(const char *token)
{
    int number;

    if (sscanf(token, "%d", &number) == 1) {
        return number_expression_create(number);
    }

    if (is_identifier(token)) {
        return variable_expression_create(token);
    }

    return 0;
}

expression_t *parse_expression(const char *text)
{
    char expr[128];
    int i;
    int op_pos = -1;
    char op = '\0';
    char left_token[64];
    char right_token[64];
    expression_t *left_expr;
    expression_t *right_expr;

    if (!trim_copy(text, expr, (int)sizeof(expr))) {
        return 0;
    }
    if (expr[0] == '\0') {
        return 0;
    }

    for (i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == '+' || expr[i] == '-') {
            op_pos = i;
            op = expr[i];
            break;
        }
    }

    if (op_pos < 0) {
        return parse_value(expr);
    }

    /* 限定只支持一个二元运算，右侧再次出现 +/- 即判非法。 */
    for (i = op_pos + 1; expr[i] != '\0'; i++) {
        if (expr[i] == '+' || expr[i] == '-') {
            return 0;
        }
    }

    if (op_pos >= (int)sizeof(left_token)) {
        return 0;
    }

    strncpy(left_token, expr, (size_t)op_pos);
    left_token[op_pos] = '\0';
    if (!trim_copy(left_token, left_token, (int)sizeof(left_token))) {
        return 0;
    }
    if (!trim_copy(expr + op_pos + 1, right_token, (int)sizeof(right_token))) {
        return 0;
    }

    left_expr = parse_value(left_token);
    right_expr = parse_value(right_token);
    if (left_expr == 0 || right_expr == 0) {
        if (left_expr != 0) {
            left_expr->destroy(left_expr);
        }
        if (right_expr != 0) {
            right_expr->destroy(right_expr);
        }
        return 0;
    }

    if (op == '+') {
        return add_expression_create(left_expr, right_expr);
    }
    return sub_expression_create(left_expr, right_expr);
}

statement_t *parse_statement(const char *line)
{
    char buf[128];
    char name[MAX_NAME_LEN];
    char rhs[96];
    char var_name[MAX_NAME_LEN];
    expression_t *expr;
    char *eq_pos;

    if (!trim_copy(line, buf, (int)sizeof(buf))) {
        return 0;
    }
    if (buf[0] == '\0') {
        return 0;
    }

    if (sscanf(buf, "PRINT %31s", var_name) == 1) {
        return print_statement_create(var_name);
    }

    if (sscanf(buf, "DEL %31s", var_name) == 1) {
        return delete_statement_create(var_name);
    }

    eq_pos = strchr(buf, '=');
    if (eq_pos != 0) {
        int left_len = (int)(eq_pos - buf);

        if (left_len <= 0 || left_len >= MAX_NAME_LEN) {
            return 0;
        }

        strncpy(name, buf, (size_t)left_len);
        name[left_len] = '\0';
        if (!trim_copy(name, name, (int)sizeof(name)) || !is_identifier(name)) {
            return 0;
        }

        if (!trim_copy(eq_pos + 1, rhs, (int)sizeof(rhs))) {
            return 0;
        }
        expr = parse_expression(rhs);
        if (expr == 0) {
            return 0;
        }
        return assign_statement_create(name, expr);
    }

    expr = parse_expression(buf);
    if (expr == 0) {
        return 0;
    }
    return expression_statement_create(expr);
}
