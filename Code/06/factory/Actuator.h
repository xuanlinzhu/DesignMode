#ifndef ACTUATOR_H
#define ACTUATOR_H

/**
 * 执行器模块抽象接口（风扇控制）
 * PC演示可输出文本、动画
 */
typedef struct Actuator Actuator;

struct Actuator {
    const char* type;  // 执行器类型名称
    int currentSpeed;  // 当前转速（0-100%）
    int isRunning;     // 运行状态
    
    /**
     * 初始化执行器
     */
    void (*init)(Actuator* self);
    
    /**
     * 设置风扇转速
     * @param speed 转速百分比（0-100）
     */
    void (*setSpeed)(Actuator* self, int speed);
    
    /**
     * 打开风扇（全速）
     */
    void (*turnOn)(Actuator* self);
    
    /**
     * 关闭风扇
     */
    void (*turnOff)(Actuator* self);
    
    /**
     * 获取当前转速
     * @return 当前转速百分比
     */
    int (*getSpeed)(Actuator* self);
    
    /**
     * 显示风扇状态（PC演示用）
     */
    void (*showStatus)(Actuator* self);
    
    /**
     * 清理资源
     */
    void (*destroy)(Actuator* self);
};

#endif // ACTUATOR_H
