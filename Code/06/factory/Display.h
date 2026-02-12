#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * 显示模块抽象接口
 * PC演示可输出文字或图形
 */
typedef struct Display Display;

struct Display {
    const char* type;  // 显示类型名称
    
    /**
     * 初始化显示模块
     */
    void (*init)(Display* self);
    
    /**
     * 显示数值
     * @param value 要显示的数值（温度）
     * @param unit 单位（如 "°C"）
     */
    void (*showValue)(Display* self, float value, const char* unit);
    
    /**
     * 显示状态信息
     * @param message 状态消息
     */
    void (*showStatus)(Display* self, const char* message);
    
    /**
     * 清理资源
     */
    void (*destroy)(Display* self);
};

#endif // DISPLAY_H
