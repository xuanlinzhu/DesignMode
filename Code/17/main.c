#include <stdio.h>
#include <stdint.h>

/*
 * 享元模式（Flyweight）嵌入式示例
 *
 * 目标：
 * 1) 对比普通实现与享元实现的存储差异
 * 2) 展示两种实现的访问路径不同，但行为输出一致
 * 3) 所有数据使用静态数组，不使用动态内存
 */

/* LED 总数量（对象数量） */
#define N 1000
/* 配置模板数量（共享配置种类） */
#define M 5

/*
 * LED 配置（可共享的“内在状态”）
 *
 * 这些字段在很多 LED 之间重复，因此适合被抽离出来共享。
 */
typedef struct {
    uint8_t mode;       /* 工作模式：常亮/闪烁/呼吸等 */
    uint16_t period_ms; /* 周期（毫秒） */
    uint8_t duty;       /* 占空比（0~100） */
    uint8_t polarity;   /* 极性 */
    uint8_t level;      /* 默认电平 */
} LedConfig;

/*
 * 普通方式：每个 LED 都“独占”一份完整配置
 *
 * 缺点：当配置重复率高时，会存储大量重复数据。
 */
typedef struct {
    LedConfig cfg;      /* 直接内嵌完整配置 */
    uint8_t runtime_state; /* 运行时状态（外在状态） */
} LedNormal;

/*
 * 享元方式：每个 LED 只保存“配置索引 + 运行时状态”
 *
 * 核心思想：
 * - 不再在对象中保存完整配置
 * - 通过 cfg_index 到共享池查配置
 */
typedef struct {
    uint8_t cfg_index;     /* 指向共享配置池的索引（0~M-1） */
    uint8_t runtime_state; /* 运行时状态（外在状态） */
} LedFlyweight;

/*
 * 配置模板集合（仅用于普通初始化时拷贝）
 * 说明：这是常量模板，不会在运行中修改。
 */
static const LedConfig config_templates[M] = {
    {0, 200, 10, 0, 0},
    {1, 500, 30, 1, 1},
    {2, 800, 50, 0, 1},
    {3, 1000, 70, 1, 0},
    {4, 1500, 90, 0, 1}
};

/*
 * 享元共享配置池（严格按要求使用 const）
 * 所有 Flyweight LED 都通过索引访问该池中的配置。
 */
static const LedConfig config_pool[M] = {
    {0, 200, 10, 0, 0},
    {1, 500, 30, 1, 1},
    {2, 800, 50, 0, 1},
    {3, 1000, 70, 1, 0},
    {4, 1500, 90, 0, 1}
};

/* 普通实现对象数组：每个元素都包含完整 cfg */
static LedNormal normal_leds[N];
/* 享元实现对象数组：每个元素仅保存 cfg_index */
static LedFlyweight flyweight_leds[N];

/*
 * 初始化普通方式 LED 数组
 *
 * 要求点：
 * - 使用 i % M 从 5 个模板中选配置
 * - 将完整配置“拷贝”到每个 LED 中
 */
static void init_normal_leds(void)
{
    int i;
    for (i = 0; i < N; ++i) {
        const uint8_t idx = (uint8_t)(i % M);
        normal_leds[i].cfg = config_templates[idx];
        normal_leds[i].runtime_state = (uint8_t)(i & 0x01u);
    }
}

/*
 * 初始化享元方式 LED 数组
 *
 * 要求点：
 * - 使用 i % M 写入 cfg_index
 * - 不复制配置数据，配置统一保存在 config_pool 中
 */
static void init_flyweight_leds(void)
{
    int i;
    for (i = 0; i < N; ++i) {
        flyweight_leds[i].cfg_index = (uint8_t)(i % M);
        flyweight_leds[i].runtime_state = (uint8_t)(i & 0x01u);
    }
}

/*
 * 普通实现行为函数
 *
 * 访问方式：直接访问 normal_leds[id].cfg.xxx
 */
static void run_normal_led(int id)
{
    const LedNormal *led = &normal_leds[id];

    printf("[Normal] id=%d mode=%u period=%u duty=%u polarity=%u level=%u runtime=%u\n",
           id,
           (unsigned int)led->cfg.mode,
           (unsigned int)led->cfg.period_ms,
           (unsigned int)led->cfg.duty,
           (unsigned int)led->cfg.polarity,
           (unsigned int)led->cfg.level,
           (unsigned int)led->runtime_state);
}

/*
 * 享元实现行为函数
 *
 * 访问方式：必须先通过 cfg_index 到 config_pool 中查配置，
 * 然后再使用配置字段。
 */
static void run_flyweight_led(int id)
{
    const LedFlyweight *led = &flyweight_leds[id];
    const LedConfig *cfg = &config_pool[led->cfg_index];

    printf("[Flyweight] id=%d mode=%u period=%u duty=%u polarity=%u level=%u runtime=%u\n",
           id,
           (unsigned int)cfg->mode,
           (unsigned int)cfg->period_ms,
           (unsigned int)cfg->duty,
           (unsigned int)cfg->polarity,
           (unsigned int)cfg->level,
           (unsigned int)led->runtime_state);
}

/*
 * 打印内存对比
 *
 * 普通方式总内存：N * sizeof(LedNormal)
 * 享元方式总内存：M * sizeof(LedConfig) + N * sizeof(LedFlyweight)
 */
static void print_memory_compare(void)
{
    const unsigned long normal_total = (unsigned long)N * (unsigned long)sizeof(LedNormal);
    const unsigned long flyweight_total =
        (unsigned long)M * (unsigned long)sizeof(LedConfig) +
        (unsigned long)N * (unsigned long)sizeof(LedFlyweight);
    const unsigned long saved_bytes = normal_total - flyweight_total;
    const double saved_percent = (normal_total == 0UL)
        ? 0.0
        : ((double)saved_bytes * 100.0) / (double)normal_total;

    printf("===== Memory Compare =====\n");
    printf("sizeof(LedConfig)    = %u bytes\n", (unsigned int)sizeof(LedConfig));
    printf("sizeof(LedNormal)    = %u bytes\n", (unsigned int)sizeof(LedNormal));
    printf("sizeof(LedFlyweight) = %u bytes\n", (unsigned int)sizeof(LedFlyweight));
    printf("Normal total         = %lu bytes\n", normal_total);
    printf("Flyweight total      = %lu bytes\n", flyweight_total);
    printf("Saved bytes          = %lu bytes\n", saved_bytes);
    printf("Saved percent        = %.2f%%\n", saved_percent);
    printf("==========================\n");
}

int main(void)
{
    int i;

    /* 先分别初始化两种实现的数据 */
    init_normal_leds();
    init_flyweight_leds();

    /* 打印内存占用对比 */
    print_memory_compare();

    /* 分别演示前 3 个 LED 的行为输出 */
    for (i = 0; i < 3; ++i) {
        run_normal_led(i);
    }

    printf("\n");

    for (i = 0; i < 3; ++i) {
        run_flyweight_led(i);
    }

    return 0;
}
