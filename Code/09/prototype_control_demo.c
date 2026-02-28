#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

extern void *malloc(unsigned long long size);
extern void free(void *ptr);
extern unsigned long long strlen(const char *str);
extern void *memcpy(void *dest, const void *src, unsigned long long n);
extern int printf(const char *format, ...);
extern int puts(const char *str);

/* 控件样式：这里故意做成统一结构，便于不同控件之间互相复用样式 */
typedef struct {
    uint32_t background_color;
    uint32_t border_color;
    uint16_t border_width;
    uint16_t radius;
    uint32_t text_color;
    uint8_t font_size;
    uint32_t padding;
    uint32_t shadow_color;
    uint8_t shadow_size;
} control_style_t;

typedef void (*control_callback_t)(void);

/* 通用控件对象：不区分具体控件类型，核心就是 style + label + callback */
typedef struct {
    control_style_t style;
    char *label;
    control_callback_t callback;
} control_t;

static char *dup_cstr(const char *src) {
    unsigned long long len;
    char *copy;

    /* 允许空指针输入，调用方可据此判断是否有文本 */
    if (src == NULL) {
        return NULL;
    }

    /* 复制 C 字符串，包含结尾 '\0' */
    len = strlen(src) + 1U;
    copy = (char *)malloc(len);
    if (copy != NULL) {
        memcpy(copy, src, len);
    }
    return copy;
}

static const control_t BUTTON_PROTOTYPE = {
    .style = {
        .background_color = 0x007BFF,
        .border_color = 0x0056B3,
        .border_width = 2,
        .radius = 5,
        .text_color = 0xFFFFFF,
        .font_size = 16,
        .padding = 10,
        .shadow_color = 0x000000,
        .shadow_size = 5,
    },
    .label = "Default Button",
    .callback = NULL,
};

static const control_t TEXTBOX_PROTOTYPE = {
    .style = {
        .background_color = 0xFFFFFF,
        .border_color = 0x000000,
        .border_width = 1,
        .radius = 3,
        .text_color = 0x000000,
        .font_size = 14,
        .padding = 5,
        .shadow_color = 0xAAAAAA,
        .shadow_size = 3,
    },
    .label = "Default Textbox",
    .callback = NULL,
};

static const control_t LABEL_PROTOTYPE = {
    .style = {
        .background_color = 0xFFFFFF,
        .border_color = 0xFFFFFF,
        .border_width = 0,
        .radius = 0,
        .text_color = 0x222222,
        .font_size = 13,
        .padding = 2,
        .shadow_color = 0x000000,
        .shadow_size = 0,
    },
    .label = "Default Label",
    .callback = NULL,
};

static const control_t SLIDER_PROTOTYPE = {
    .style = {
        .background_color = 0xE9ECEF,
        .border_color = 0xCED4DA,
        .border_width = 1,
        .radius = 6,
        .text_color = 0x212529,
        .font_size = 12,
        .padding = 4,
        .shadow_color = 0x999999,
        .shadow_size = 1,
    },
    .label = "Default Slider",
    .callback = NULL,
};

static const control_t CHECKBOX_PROTOTYPE = {
    .style = {
        .background_color = 0xFFFFFF,
        .border_color = 0x6C757D,
        .border_width = 1,
        .radius = 2,
        .text_color = 0x212529,
        .font_size = 13,
        .padding = 4,
        .shadow_color = 0x999999,
        .shadow_size = 1,
    },
    .label = "Default Checkbox",
    .callback = NULL,
};

static int control_create(control_t *out,
                          const control_t *prototype,
                          const char *label,
                          control_callback_t callback) {

    /* 输出对象不能为空 */
    if (out == NULL) {
        return -1;
    }

    /* 原型对象不能为空 */
    if (prototype == NULL) {
        return -1;
    }

    /* 先整体复制统一字段 */
    *out = *prototype;
    /* label 支持覆盖，未提供时沿用原型 label */
    out->label = dup_cstr((label != NULL) ? label : prototype->label);
    if (out->label == NULL) {
        return -1;
    }

    /* callback 明确由调用方决定，避免不小心继承旧行为 */
    out->callback = callback;
    return 0;
}

/* clone 本质就是从现有对象再次创建一份 */
static int control_clone(control_t *out, const control_t *src) {
    return control_create(out, src, src->label, src->callback);
}

/* 关键演示函数：把 source 的 style 传递给 target（跨控件可用） */
static void control_transfer_style(control_t *target, const control_t *source) {
    if ((target == NULL) || (source == NULL)) {
        return;
    }
    target->style = source->style;
}

static void control_destroy(control_t *control) {
    /* 析构函数要能安全重复调用 */
    if (control == NULL) {
        return;
    }

    /* 只释放自己拥有的动态内存 */
    free(control->label);
    control->label = NULL;
    control->callback = NULL;
}

static void print_color_chip(const char *name, uint32_t color) {
    unsigned int r = (unsigned int)((color >> 16) & 0xFFU);
    unsigned int g = (unsigned int)((color >> 8) & 0xFFU);
    unsigned int b = (unsigned int)(color & 0xFFU);

    printf("  %-6s=0x%06X ", name, color);
    printf("\x1b[48;2;%u;%u;%um  \x1b[0m", r, g, b);
    printf("\x1b[38;2;%u;%u;%um  sample\x1b[0m\n", r, g, b);
}

/* 打印函数用于观察样式是否真的完成了跨控件传递 */
static void print_control(const control_t *control, const char *title) {
    if (control == NULL) {
        return;
    }

    printf("[%s]\n", title);
    printf("  label=%s\n", (control->label != NULL) ? control->label : "(null)");
    printf("  border_width=%u radius=%u font=%u padding=%u shadow_size=%u\n",
           control->style.border_width,
           control->style.radius,
           control->style.font_size,
           control->style.padding,
           control->style.shadow_size);
    print_color_chip("bg", control->style.background_color);
    print_color_chip("border", control->style.border_color);
    print_color_chip("text", control->style.text_color);
    print_color_chip("shadow", control->style.shadow_color);
    puts("");
}

static void on_submit(void) {
    puts("callback: submit clicked");
}

int main(void) {
    /* 基础对象：按钮 */
    control_t submit_btn;
    /* 从按钮 clone 出来的危险按钮 */
    control_t danger_btn;
    /* 基础对象：输入框 */
    control_t name_input;
    /* 从输入框 clone 出来的勾选框视觉 */
    control_t agree_checkbox;
    /* 原生 checkbox 原型实例 */
    control_t legal_checkbox;
    /* 从 slider 原型派生出的“标签风格”控件 */
    control_t caption_from_slider;
    /* 从 label 原型派生出的“滑块风格”控件 */
    control_t slider_from_label;
    /* 样式传递示例 1：按钮接收输入框样式 */
    control_t button_take_textbox_style;
    /* 样式传递示例 2：文本框接收勾选框样式 */
    control_t textbox_take_checkbox_style;
    /* 样式传递示例 3：标签接收按钮样式 */
    control_t label_take_button_style;

    /* 1) 创建 submit 按钮 */
    if (control_create(&submit_btn, &BUTTON_PROTOTYPE, "Submit", on_submit) != 0) {
        puts("create submit button failed");
        return 1;
    }

    /* 2) clone 出 danger 按钮，并做局部样式差异化 */
    if (control_clone(&danger_btn, &submit_btn) != 0) {
        puts("clone button failed");
        control_destroy(&submit_btn);
        return 1;
    }

    free(danger_btn.label);
    danger_btn.label = dup_cstr("Delete");
    if (danger_btn.label == NULL) {
        puts("set clone label failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        return 1;
    }
    danger_btn.style.background_color = 0xDC3545;
    danger_btn.style.border_color = 0xA71D2A;

    /* 3) 创建输入框 */
    if (control_create(&name_input, &TEXTBOX_PROTOTYPE, "Name", NULL) != 0) {
        puts("create textbox failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        return 1;
    }
    name_input.style.radius = 8;
    name_input.style.padding = 8;

    /* 4) 跨控件 clone：输入框 -> 勾选框风格对象 */
    if (control_clone(&agree_checkbox, &name_input) != 0) {
        puts("clone textbox to checkbox failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        control_destroy(&name_input);
        return 1;
    }
    free(agree_checkbox.label);
    agree_checkbox.label = dup_cstr("Agree Terms");
    if (agree_checkbox.label == NULL) {
        puts("set checkbox label failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        control_destroy(&name_input);
        control_destroy(&agree_checkbox);
        return 1;
    }
    agree_checkbox.style.border_color = 0x198754;
    agree_checkbox.style.background_color = 0xF0FFF4;
    agree_checkbox.style.radius = 2;

    /* 5) 常规 checkbox 原型实例 */
    if (control_create(&legal_checkbox, &CHECKBOX_PROTOTYPE, "I accept policy", NULL) != 0) {
        puts("create checkbox failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        control_destroy(&name_input);
        control_destroy(&agree_checkbox);
        return 1;
    }
    legal_checkbox.style.padding = 6;

    /* 6) 跨控件派生：slider 原型 -> 偏 label 的展示 */
    if (control_create(&caption_from_slider, &SLIDER_PROTOTYPE, "Volume", NULL) != 0) {
        puts("create slider caption failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        control_destroy(&name_input);
        control_destroy(&agree_checkbox);
        control_destroy(&legal_checkbox);
        return 1;
    }
    caption_from_slider.style.border_width = 0;
    caption_from_slider.style.padding = 1;
    caption_from_slider.style.background_color = 0xFFFFFF;

    /* 7) 跨控件派生：label 原型 -> 偏 slider 的展示 */
    if (control_create(&slider_from_label, &LABEL_PROTOTYPE, "Brightness", NULL) != 0) {
        puts("create label-based slider failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        control_destroy(&name_input);
        control_destroy(&agree_checkbox);
        control_destroy(&legal_checkbox);
        control_destroy(&caption_from_slider);
        return 1;
    }
    slider_from_label.style.border_width = 1;
    slider_from_label.style.border_color = 0xADB5BD;
    slider_from_label.style.background_color = 0xE9ECEF;
    slider_from_label.style.radius = 6;
    slider_from_label.style.padding = 4;

    /* 8) 明确演示“style 传递”而不仅是 clone */
    if (control_create(&button_take_textbox_style, &BUTTON_PROTOTYPE, "Button <- Textbox Style", NULL) != 0) {
        puts("create button_take_textbox_style failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        control_destroy(&name_input);
        control_destroy(&agree_checkbox);
        control_destroy(&legal_checkbox);
        control_destroy(&caption_from_slider);
        control_destroy(&slider_from_label);
        return 1;
    }
    control_transfer_style(&button_take_textbox_style, &name_input);

    if (control_create(&textbox_take_checkbox_style, &TEXTBOX_PROTOTYPE, "Textbox <- Checkbox Style", NULL) != 0) {
        puts("create textbox_take_checkbox_style failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        control_destroy(&name_input);
        control_destroy(&agree_checkbox);
        control_destroy(&legal_checkbox);
        control_destroy(&caption_from_slider);
        control_destroy(&slider_from_label);
        control_destroy(&button_take_textbox_style);
        return 1;
    }
    control_transfer_style(&textbox_take_checkbox_style, &legal_checkbox);

    if (control_create(&label_take_button_style, &LABEL_PROTOTYPE, "Label <- Danger Button Style", NULL) != 0) {
        puts("create label_take_button_style failed");
        control_destroy(&submit_btn);
        control_destroy(&danger_btn);
        control_destroy(&name_input);
        control_destroy(&agree_checkbox);
        control_destroy(&legal_checkbox);
        control_destroy(&caption_from_slider);
        control_destroy(&slider_from_label);
        control_destroy(&button_take_textbox_style);
        control_destroy(&textbox_take_checkbox_style);
        return 1;
    }
    control_transfer_style(&label_take_button_style, &danger_btn);

    /* 9) 打印所有对象，观察样式复用效果 */
    print_control(&submit_btn, "Prototype Instance 1");
    print_control(&danger_btn, "Cloned + Customized Instance");
    print_control(&name_input, "Prototype Instance 2");
    print_control(&agree_checkbox, "Cross Clone: Textbox -> Checkbox Style");
    print_control(&legal_checkbox, "Prototype Instance 3");
    print_control(&caption_from_slider, "Cross Use: Slider Prototype -> Label Style");
    print_control(&slider_from_label, "Cross Use: Label Prototype -> Slider Style");
    print_control(&button_take_textbox_style, "Style Transfer: Button <- Textbox");
    print_control(&textbox_take_checkbox_style, "Style Transfer: Textbox <- Checkbox");
    print_control(&label_take_button_style, "Style Transfer: Label <- Button");

    /* 10) 回调验证 */
    if (submit_btn.callback != NULL) {
        submit_btn.callback();
    }

    /* 11) 统一销毁，防止内存泄漏 */
    control_destroy(&submit_btn);
    control_destroy(&danger_btn);
    control_destroy(&name_input);
    control_destroy(&agree_checkbox);
    control_destroy(&legal_checkbox);
    control_destroy(&caption_from_slider);
    control_destroy(&slider_from_label);
    control_destroy(&button_take_textbox_style);
    control_destroy(&textbox_take_checkbox_style);
    control_destroy(&label_take_button_style);
    return 0;
}
