#include "menu_component.h"

extern int printf(const char *format, ...);

/* 打印缩进：用于树形结构展示 */
static void print_indent(int depth)
{
    int i;
    for (i = 0; i < depth; ++i) {
        printf("  ");
    }
}

static void composite_print(const menu_component_t *node, int depth);
static void composite_execute(const menu_component_t *node);
static void composite_traverse(const menu_component_t *node, int depth, menu_visit_fn visitor, void *ctx);

static void info_leaf_print(const menu_component_t *node, int depth);
static void info_leaf_execute(const menu_component_t *node);
static void leaf_traverse(const menu_component_t *node, int depth, menu_visit_fn visitor, void *ctx);

static void action_leaf_print(const menu_component_t *node, int depth);
static void action_leaf_execute(const menu_component_t *node);

static const menu_component_ops_t g_composite_ops = {
    composite_print,
    composite_execute,
    composite_traverse
};

static const menu_component_ops_t g_info_leaf_ops = {
    info_leaf_print,
    info_leaf_execute,
    leaf_traverse
};

static const menu_component_ops_t g_action_leaf_ops = {
    action_leaf_print,
    action_leaf_execute,
    leaf_traverse
};

/*
 * Composite 初始化：
 * 1) 绑定 Composite 操作表
 * 2) 清零子节点计数
 * 3) 将固定容量数组置空
 */
void menu_composite_init(menu_composite_t *node, const char *name)
{
    size_t i;
    if (node == NULL) {
        return;
    }

    node->base.ops = &g_composite_ops;
    node->base.type = MENU_NODE_COMPOSITE;
    node->base.name = name;
    node->base.parent = NULL;
    node->child_count = 0U;

    for (i = 0U; i < MENU_MAX_CHILDREN; ++i) {
        node->children[i] = NULL;
    }
}

void menu_info_leaf_init(menu_leaf_t *node, const char *name, const char *info_text)
{
    if (node == NULL) {
        return;
    }

    node->base.ops = &g_info_leaf_ops;
    node->base.type = MENU_NODE_INFO;
    node->base.name = name;
    node->base.parent = NULL;
    node->payload = info_text;
}

void menu_action_leaf_init(menu_leaf_t *node, const char *name, const char *action_text)
{
    if (node == NULL) {
        return;
    }

    node->base.ops = &g_action_leaf_ops;
    node->base.type = MENU_NODE_ACTION;
    node->base.name = name;
    node->base.parent = NULL;
    node->payload = action_text;
}

int menu_composite_add(menu_composite_t *parent, menu_component_t *child)
{
    /* 容错：父子任一为空，直接失败 */
    if (parent == NULL || child == NULL) {
        return -1;
    }

    /* 固定容量保护：禁止越界写入 */
    if (parent->child_count >= MENU_MAX_CHILDREN) {
        return -1;
    }

    /* 建立父子关系：既挂载 child，也回写 child->parent */
    parent->children[parent->child_count] = child;
    parent->child_count += 1U;
    child->parent = &parent->base;
    return 0;
}

void menu_component_print_tree(const menu_component_t *node)
{
    /* 统一入口：通过多态表下发到真实类型 */
    if (node == NULL || node->ops == NULL || node->ops->print == NULL) {
        return;
    }

    node->ops->print(node, 0);
}

void menu_component_execute(const menu_component_t *node)
{
    /* 统一入口：不区分 Composite 或 Leaf */
    if (node == NULL || node->ops == NULL || node->ops->execute == NULL) {
        return;
    }

    node->ops->execute(node);
}

void menu_component_traverse(const menu_component_t *node, menu_visit_fn visitor, void *ctx)
{
    /* 统一入口：深度优先递归由具体类型实现 */
    if (node == NULL || node->ops == NULL || node->ops->traverse == NULL) {
        return;
    }

    node->ops->traverse(node, 0, visitor, ctx);
}

int menu_component_is_composite(const menu_component_t *node)
{
    return (node != NULL && node->type == MENU_NODE_COMPOSITE) ? 1 : 0;
}

const menu_component_t *menu_component_parent(const menu_component_t *node)
{
    if (node == NULL) {
        return NULL;
    }
    return node->parent;
}

size_t menu_component_child_count(const menu_component_t *node)
{
    const menu_composite_t *comp;
    if (!menu_component_is_composite(node)) {
        return 0U;
    }

    comp = (const menu_composite_t *)node;
    return comp->child_count;
}

const menu_component_t *menu_component_child_at(const menu_component_t *node, size_t index)
{
    const menu_composite_t *comp;
    if (!menu_component_is_composite(node)) {
        return NULL;
    }

    comp = (const menu_composite_t *)node;
    if (index >= comp->child_count) {
        return NULL;
    }

    return comp->children[index];
}

static void composite_print(const menu_component_t *node, int depth)
{
    size_t i;
    const menu_composite_t *comp = (const menu_composite_t *)node;

    print_indent(depth);
    printf("[Composite] %s\n", node->name);

    /* Composite 的关键：把同一操作递归传播给全部子节点 */
    for (i = 0U; i < comp->child_count; ++i) {
        const menu_component_t *child = comp->children[i];
        if (child != NULL && child->ops != NULL && child->ops->print != NULL) {
            child->ops->print(child, depth + 1);
        }
    }
}

static void composite_execute(const menu_component_t *node)
{
    /* 课堂演示语义：Composite 的 execute 表示“进入菜单” */
    printf("[EXEC][Composite] Enter menu: %s\n", node->name);
}

static void composite_traverse(const menu_component_t *node, int depth, menu_visit_fn visitor, void *ctx)
{
    size_t i;
    const menu_composite_t *comp = (const menu_composite_t *)node;

    /* 前序访问当前 Composite */
    if (visitor != NULL) {
        visitor(node, depth, ctx);
    }

    /* 深度优先遍历子树 */
    for (i = 0U; i < comp->child_count; ++i) {
        const menu_component_t *child = comp->children[i];
        if (child != NULL && child->ops != NULL && child->ops->traverse != NULL) {
            child->ops->traverse(child, depth + 1, visitor, ctx);
        }
    }
}

static void info_leaf_print(const menu_component_t *node, int depth)
{
    const menu_leaf_t *leaf = (const menu_leaf_t *)node;
    print_indent(depth);
    printf("[Info] %s\n", node->name);
    print_indent(depth + 1);
    /* 信息叶子：显示 name + 固定文本 */
    printf("=> %s\n", (leaf->payload != NULL) ? leaf->payload : "(empty)");
}

static void info_leaf_execute(const menu_component_t *node)
{
    const menu_leaf_t *leaf = (const menu_leaf_t *)node;
    printf("[EXEC][Info] %s: %s\n",
           node->name,
           (leaf->payload != NULL) ? leaf->payload : "(empty)");
}

static void action_leaf_print(const menu_component_t *node, int depth)
{
    const menu_leaf_t *leaf = (const menu_leaf_t *)node;
    print_indent(depth);
    printf("[Action] %s\n", node->name);
    print_indent(depth + 1);
    printf("=> %s\n", (leaf->payload != NULL) ? leaf->payload : "(empty)");
}

static void action_leaf_execute(const menu_component_t *node)
{
    const menu_leaf_t *leaf = (const menu_leaf_t *)node;
    /* 动作叶子：执行时输出动作日志 */
    printf("[EXEC][Action] %s -> %s\n",
           node->name,
           (leaf->payload != NULL) ? leaf->payload : "done");
}

static void leaf_traverse(const menu_component_t *node, int depth, menu_visit_fn visitor, void *ctx)
{
    /* Leaf 为递归终点：只访问自身，不再向下递归 */
    if (visitor != NULL) {
        visitor(node, depth, ctx);
    }
}
