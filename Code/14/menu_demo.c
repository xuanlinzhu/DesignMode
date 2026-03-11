#include "menu_component.h"

extern int printf(const char *format, ...);
extern int puts(const char *str);
extern int getchar(void);

typedef struct {
    /* 遍历计数器：用于演示递归访问总节点数 */
    int visited;
} traverse_ctx_t;

/*
 * 读取一行输入（兼容 CRLF/LF）：
 * - 遇到 '\n' 结束
 * - 自动补 '\0'
 * - 超长输入将被截断到 cap-1
 */
static int read_line(char *buf, int cap)
{
    int c;
    int len = 0;

    if (buf == 0 || cap <= 1) {
        return 0;
    }

    while ((c = getchar()) != -1) {
        if (c == '\r') {
            continue;
        }
        if (c == '\n') {
            break;
        }
        if (len < cap - 1) {
            buf[len] = (char)c;
            len += 1;
        }
    }

    buf[len] = '\0';
    return len;
}

static int parse_index(const char *s, int *out)
{
    int i = 0;
    int value = 0;
    int has_digit = 0;

    if (s == 0 || out == 0) {
        return -1;
    }

    /* 跳过前导空白，便于用户输入 "  3" */
    while (s[i] == ' ' || s[i] == '\t') {
        i += 1;
    }

    /* 仅解析十进制数字前缀 */
    while (s[i] >= '0' && s[i] <= '9') {
        has_digit = 1;
        value = value * 10 + (s[i] - '0');
        i += 1;
    }

    if (!has_digit) {
        return -1;
    }

    *out = value;
    return 0;
}

static int safe_add(menu_composite_t *parent, menu_component_t *child)
{
    /* 统一构树错误提示，避免 main 中重复打印逻辑 */
    if (menu_composite_add(parent, child) != 0) {
        printf("build error: failed to add '%s' under '%s'\n", child->name, parent->base.name);
        return -1;
    }
    return 0;
}

static void print_path(const menu_component_t *node)
{
    const menu_component_t *stack[16];
    int top = 0;
    int i;
    const menu_component_t *cur = node;

    /* 先沿 parent 回溯到根，再逆序打印完整路径 */
    while (cur != 0 && top < 16) {
        stack[top] = cur;
        top += 1;
        cur = menu_component_parent(cur);
    }

    printf("Path: ");
    for (i = top - 1; i >= 0; --i) {
        printf("/%s", stack[i]->name);
    }
    printf("\n");
}

static void traverse_printer(const menu_component_t *node, int depth, void *ctx)
{
    int i;
    traverse_ctx_t *state = (traverse_ctx_t *)ctx;

    /* 访问回调里做累计统计 */
    if (state != 0) {
        state->visited += 1;
    }

    for (i = 0; i < depth; ++i) {
        printf("  ");
    }

    /* 按节点类型打印标签，便于课堂演示 Component 统一访问 */
    if (node->type == MENU_NODE_COMPOSITE) {
        printf("- [Composite] %s\n", node->name);
    } else if (node->type == MENU_NODE_INFO) {
        printf("- [Info] %s\n", node->name);
    } else {
        printf("- [Action] %s\n", node->name);
    }
}

static void print_role_mapping(void)
{
    /* 对应关系说明：直接映射 GOF 术语 */
    puts("\n=== Composite Pattern Mapping ===");
    puts("Component : menu_component_t + menu_component_ops_t");
    puts("Composite : menu_composite_t (contains children)");
    puts("Leaf      : menu_leaf_t (info/action terminal nodes)");
    puts("Client    : menu_demo.c (builds tree and drives operations)");
}

static void interactive_menu(const menu_component_t *root)
{
    const menu_component_t *current = root;
    char line[64];

    puts("\n=== Interactive Menu ===");
    puts("Type a number to enter child, '0' to go back, 'e' to execute, 't' to print subtree, 'q' to quit.");

    /* 交互循环：支持进入子节点、返回父节点、执行、打印子树、退出 */
    while (1) {
        size_t count;
        size_t i;
        int index;

        puts("\n--------------------------------");
        print_path(current);

        if (menu_component_is_composite(current)) {
            /* Composite 节点展示其子菜单列表 */
            count = menu_component_child_count(current);
            printf("Current menu: %s (children=%lu)\n", current->name, (unsigned long)count);
            for (i = 0U; i < count; ++i) {
                const menu_component_t *child = menu_component_child_at(current, i);
                const char *type_name = "Action";
                if (child->type == MENU_NODE_COMPOSITE) {
                    type_name = "Composite";
                } else if (child->type == MENU_NODE_INFO) {
                    type_name = "Info";
                }
                printf("  %lu) [%s] %s\n", (unsigned long)(i + 1U), type_name, child->name);
            }
        } else {
            /* Leaf 节点无子项 */
            printf("Current leaf: %s\n", current->name);
        }

        printf("Input> ");
        if (read_line(line, (int)sizeof(line)) <= 0) {
            continue;
        }

        if (line[0] == 'q' || line[0] == 'Q') {
            puts("leave interactive mode");
            break;
        }

        if (line[0] == 'e' || line[0] == 'E') {
            /* 统一执行入口：不需要区分 Composite/Leaf */
            menu_component_execute(current);
            continue;
        }

        if (line[0] == 't' || line[0] == 'T') {
            /* 打印当前节点为根的子树 */
            menu_component_print_tree(current);
            continue;
        }

        if (parse_index(line, &index) == 0) {
            if (index == 0) {
                /* 0 代表返回上级 */
                const menu_component_t *parent = menu_component_parent(current);
                if (parent != 0) {
                    current = parent;
                } else {
                    puts("already at root");
                }
                continue;
            }

            if (!menu_component_is_composite(current)) {
                puts("current node is leaf; use 0 to go back");
                continue;
            }

            if ((size_t)index > menu_component_child_count(current)) {
                puts("invalid index");
                continue;
            }

            current = menu_component_child_at(current, (size_t)(index - 1));
            continue;
        }

        puts("unknown command");
    }
}

int main(void)
{
    /*
     * 静态对象建树：
     * 题目要求不使用动态内存，因此所有 Composite/Leaf 均为栈上静态对象。
     */
    menu_composite_t main_menu;
    menu_composite_t device_info;
    menu_composite_t settings;
    menu_composite_t display;
    menu_composite_t language;
    menu_composite_t network;
    menu_composite_t power;
    menu_composite_t diagnostics;
    menu_composite_t control;
    menu_composite_t about;

    menu_leaf_t model;
    menu_leaf_t serial_number;
    menu_leaf_t firmware_version;
    menu_leaf_t hardware_version;
    menu_leaf_t brightness;
    menu_leaf_t contrast;
    menu_leaf_t chinese;
    menu_leaf_t english;
    menu_leaf_t ip_address;
    menu_leaf_t subnet_mask;
    menu_leaf_t gateway;
    menu_leaf_t dhcp;
    menu_leaf_t sleep_timeout;
    menu_leaf_t auto_shutdown;
    menu_leaf_t low_power_mode;
    menu_leaf_t restore_defaults;
    menu_leaf_t sensor_status;
    menu_leaf_t memory_test;
    menu_leaf_t storage_test;
    menu_leaf_t network_ping;
    menu_leaf_t export_log;
    menu_leaf_t start_service;
    menu_leaf_t stop_service;
    menu_leaf_t restart_service;
    menu_leaf_t emergency_stop;
    menu_leaf_t license;
    menu_leaf_t build_time;

    traverse_ctx_t tr_ctx;

    menu_composite_init(&main_menu, "Main");
    menu_composite_init(&device_info, "Device Info");
    menu_composite_init(&settings, "Settings");
    menu_composite_init(&display, "Display");
    menu_composite_init(&language, "Language");
    menu_composite_init(&network, "Network");
    menu_composite_init(&power, "Power");
    menu_composite_init(&diagnostics, "Diagnostics");
    menu_composite_init(&control, "Control");
    menu_composite_init(&about, "About");

    menu_info_leaf_init(&model, "Model", "DM-500 Embedded Controller");
    menu_info_leaf_init(&serial_number, "Serial Number", "SN-2026-0311-0007");
    menu_info_leaf_init(&firmware_version, "Firmware Version", "v2.4.1");
    menu_info_leaf_init(&hardware_version, "Hardware Version", "Rev-C");
    menu_action_leaf_init(&brightness, "Brightness", "set display brightness");
    menu_action_leaf_init(&contrast, "Contrast", "set display contrast");
    menu_action_leaf_init(&chinese, "Chinese", "switch language to Chinese");
    menu_action_leaf_init(&english, "English", "switch language to English");
    menu_action_leaf_init(&ip_address, "IP Address", "configure static IP address");
    menu_action_leaf_init(&subnet_mask, "Subnet Mask", "configure subnet mask");
    menu_action_leaf_init(&gateway, "Gateway", "configure default gateway");
    menu_action_leaf_init(&dhcp, "DHCP", "toggle DHCP mode");
    menu_action_leaf_init(&sleep_timeout, "Sleep Timeout", "set sleep timeout");
    menu_action_leaf_init(&auto_shutdown, "Auto Shutdown", "set auto shutdown policy");
    menu_action_leaf_init(&low_power_mode, "Low Power Mode", "toggle low power mode");
    menu_action_leaf_init(&restore_defaults, "Restore Defaults", "restore all settings to defaults");
    menu_info_leaf_init(&sensor_status, "Sensor Status", "all sensors healthy");
    menu_action_leaf_init(&memory_test, "Memory Test", "run RAM diagnostic");
    menu_action_leaf_init(&storage_test, "Storage Test", "run storage self-check");
    menu_action_leaf_init(&network_ping, "Network Ping", "ping gateway 192.168.1.1");
    menu_action_leaf_init(&export_log, "Export Log", "export logs to USB storage");
    menu_action_leaf_init(&start_service, "Start Service", "service started");
    menu_action_leaf_init(&stop_service, "Stop Service", "service stopped");
    menu_action_leaf_init(&restart_service, "Restart Service", "service restarted");
    menu_action_leaf_init(&emergency_stop, "Emergency Stop", "emergency stop activated");
    menu_info_leaf_init(&license, "License", "Demo License - Education Use Only");
    menu_info_leaf_init(&build_time, "Build Time", __DATE__ " " __TIME__);

    /* ===== 按设计说明中的层级关系构建完整菜单树 ===== */
    if (safe_add(&main_menu, &device_info.base) != 0) return 1;
    if (safe_add(&main_menu, &settings.base) != 0) return 1;
    if (safe_add(&main_menu, &diagnostics.base) != 0) return 1;
    if (safe_add(&main_menu, &control.base) != 0) return 1;
    if (safe_add(&main_menu, &about.base) != 0) return 1;

    if (safe_add(&device_info, &model.base) != 0) return 1;
    if (safe_add(&device_info, &serial_number.base) != 0) return 1;
    if (safe_add(&device_info, &firmware_version.base) != 0) return 1;
    if (safe_add(&device_info, &hardware_version.base) != 0) return 1;

    if (safe_add(&settings, &display.base) != 0) return 1;
    if (safe_add(&settings, &network.base) != 0) return 1;
    if (safe_add(&settings, &power.base) != 0) return 1;
    if (safe_add(&settings, &restore_defaults.base) != 0) return 1;

    if (safe_add(&display, &brightness.base) != 0) return 1;
    if (safe_add(&display, &contrast.base) != 0) return 1;
    if (safe_add(&display, &language.base) != 0) return 1;

    if (safe_add(&language, &chinese.base) != 0) return 1;
    if (safe_add(&language, &english.base) != 0) return 1;

    if (safe_add(&network, &ip_address.base) != 0) return 1;
    if (safe_add(&network, &subnet_mask.base) != 0) return 1;
    if (safe_add(&network, &gateway.base) != 0) return 1;
    if (safe_add(&network, &dhcp.base) != 0) return 1;

    if (safe_add(&power, &sleep_timeout.base) != 0) return 1;
    if (safe_add(&power, &auto_shutdown.base) != 0) return 1;
    if (safe_add(&power, &low_power_mode.base) != 0) return 1;

    if (safe_add(&diagnostics, &sensor_status.base) != 0) return 1;
    if (safe_add(&diagnostics, &memory_test.base) != 0) return 1;
    if (safe_add(&diagnostics, &storage_test.base) != 0) return 1;
    if (safe_add(&diagnostics, &network_ping.base) != 0) return 1;
    if (safe_add(&diagnostics, &export_log.base) != 0) return 1;

    if (safe_add(&control, &start_service.base) != 0) return 1;
    if (safe_add(&control, &stop_service.base) != 0) return 1;
    if (safe_add(&control, &restart_service.base) != 0) return 1;
    if (safe_add(&control, &emergency_stop.base) != 0) return 1;

    if (safe_add(&about, &license.base) != 0) return 1;
    if (safe_add(&about, &build_time.base) != 0) return 1;

    /* ===== 自动演示步骤 ===== */
    puts("=== Composite Pattern Demo: Embedded Menu Tree ===");
    print_role_mapping();

    puts("\n[Step 1] Print full menu tree");
    menu_component_print_tree(&main_menu.base);

    puts("\n[Step 2] Unified execute: one Composite + one Info Leaf + one Action Leaf");
    /* 统一接口演示：客户端只调用 menu_component_execute */
    menu_component_execute(&settings.base);
    menu_component_execute(&model.base);
    menu_component_execute(&start_service.base);

    puts("\n[Step 3] Recursive traversal of whole tree");
    tr_ctx.visited = 0;
    menu_component_traverse(&main_menu.base, traverse_printer, &tr_ctx);
    printf("Traversal done, visited nodes = %d\n", tr_ctx.visited);

    puts("\n[Step 4] Interactive navigation");
    interactive_menu(&main_menu.base);

    puts("\nWhy this is Composite: client always holds menu_component_t pointer, and recursive operations are delegated by node type.");
    puts("Compile: gcc -std=c99 -Wall -Wextra -pedantic menu_component.c menu_demo.c -o menu_demo");
    puts("Run    : ./menu_demo");
    return 0;
}
