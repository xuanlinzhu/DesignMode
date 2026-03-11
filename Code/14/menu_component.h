#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 每个 Composite 节点的固定子节点容量。
 * 说明：题目要求静态分配，因此这里使用编译期常量数组，不使用动态内存。
 */
#define MENU_MAX_CHILDREN 8U

/*
 * 节点类型：
 * - COMPOSITE：目录/菜单分组，可包含子节点
 * - INFO     ：信息叶子节点，执行时输出固定信息
 * - ACTION   ：动作叶子节点，执行时输出动作日志
 */
typedef enum {
    MENU_NODE_COMPOSITE = 0,
    MENU_NODE_INFO = 1,
    MENU_NODE_ACTION = 2
} menu_node_type_t;

typedef struct menu_component menu_component_t;
typedef struct menu_composite menu_composite_t;
typedef struct menu_leaf menu_leaf_t;

/*
 * 递归遍历回调签名：
 * node  : 当前访问节点
 * depth : 当前深度（根节点为 0）
 * ctx   : 用户上下文（用于统计/聚合等）
 */
typedef void (*menu_visit_fn)(const menu_component_t *node, int depth, void *ctx);

/*
 * Component 多态操作表：
 * print    : 打印当前节点（Composite 会递归打印子树）
 * execute  : 执行当前节点语义
 * traverse : 递归遍历当前节点（深度优先）
 */
typedef struct {
    void (*print)(const menu_component_t *node, int depth);
    void (*execute)(const menu_component_t *node);
    void (*traverse)(const menu_component_t *node, int depth, menu_visit_fn visitor, void *ctx);
} menu_component_ops_t;

/*
 * 统一抽象 Component。
 * 所有节点都以该结构作为“基类”，客户端只需持有 menu_component_t* 即可统一处理。
 */
struct menu_component {
    const menu_component_ops_t *ops;
    menu_node_type_t type;
    const char *name;
    const menu_component_t *parent;
};

/*
 * Composite：可包含子节点的分组节点。
 * children   : 固定容量子节点数组
 * child_count: 当前已挂载子节点数量
 */
struct menu_composite {
    menu_component_t base;
    menu_component_t *children[MENU_MAX_CHILDREN];
    size_t child_count;
};

/*
 * Leaf：终端节点。
 * payload 在 INFO/ACTION 两类叶子上分别表示“信息文本/动作描述”。
 */
struct menu_leaf {
    menu_component_t base;
    const char *payload;
};

/* 初始化 Composite 节点 */
void menu_composite_init(menu_composite_t *node, const char *name);
/* 初始化信息叶子节点 */
void menu_info_leaf_init(menu_leaf_t *node, const char *name, const char *info_text);
/* 初始化动作叶子节点 */
void menu_action_leaf_init(menu_leaf_t *node, const char *name, const char *action_text);

/* 向 Composite 添加一个子节点，成功返回 0，失败返回 -1 */
int menu_composite_add(menu_composite_t *parent, menu_component_t *child);

/* 统一接口：打印整棵子树 */
void menu_component_print_tree(const menu_component_t *node);
/* 统一接口：执行节点 */
void menu_component_execute(const menu_component_t *node);
/* 统一接口：遍历子树 */
void menu_component_traverse(const menu_component_t *node, menu_visit_fn visitor, void *ctx);

/* 工具函数：判断节点是否为 Composite */
int menu_component_is_composite(const menu_component_t *node);
/* 工具函数：获取父节点 */
const menu_component_t *menu_component_parent(const menu_component_t *node);
/* 工具函数：获取子节点数量（非 Composite 返回 0） */
size_t menu_component_child_count(const menu_component_t *node);
/* 工具函数：按索引获取子节点（越界返回 NULL） */
const menu_component_t *menu_component_child_at(const menu_component_t *node, size_t index);

#ifdef __cplusplus
}
#endif
