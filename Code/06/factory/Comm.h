#ifndef COMM_H
#define COMM_H

/**
 * 通信模块抽象接口
 * PC演示可输出日志或模拟传输
 */
typedef struct Comm Comm;

struct Comm {
    const char* type;  // 通信类型名称
    
    /**
     * 初始化通信模块
     */
    void (*init)(Comm* self);
    
    /**
     * 发送数据
     * @param data 要发送的数据
     * @param length 数据长度
     * @return 成功发送的字节数
     */
    int (*send)(Comm* self, const char* data, int length);
    
    /**
     * 接收数据
     * @param buffer 接收缓冲区
     * @param maxLength 最大接收长度
     * @return 实际接收的字节数
     */
    int (*receive)(Comm* self, char* buffer, int maxLength);
    
    /**
     * 检查连接状态
     * @return 1表示已连接，0表示未连接
     */
    int (*isConnected)(Comm* self);
    
    /**
     * 清理资源
     */
    void (*destroy)(Comm* self);
};

#endif // COMM_H
