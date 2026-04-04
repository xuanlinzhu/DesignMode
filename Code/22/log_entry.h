#ifndef LOG_ENTRY_H
#define LOG_ENTRY_H

/* 所有日志容器共用的统一日志结构。 */
typedef struct
{
    int id;
    int level;
    char message[64];
} log_entry_t;

#endif
