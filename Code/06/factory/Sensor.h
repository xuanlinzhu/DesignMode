#ifndef SENSOR_H
#define SENSOR_H

/**
 * 传感器模块抽象接口
 * PC演示可生成随机或模拟温度值
 */
typedef struct Sensor Sensor;

struct Sensor {
    const char* type;  // 传感器类型名称
    float minValue;    // 最小测量值
    float maxValue;    // 最大测量值
    
    /**
     * 读取温度值
     * @return 当前温度值
     */
    float (*readValue)(Sensor* self);
    
    /**
     * 获取传感器精度
     * @return 精度值（如 0.1 表示精确到小数点后1位）
     */
    float (*getPrecision)(Sensor* self);
    
    /**
     * 初始化传感器
     */
    void (*init)(Sensor* self);
    
    /**
     * 清理资源
     */
    void (*destroy)(Sensor* self);
};

#endif // SENSOR_H
