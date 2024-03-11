/* 缓存日志类型定义 */

#ifndef CACHE_LOG
#define CACHE_LOG

#include <cstdint>

enum log_t {
    EVICT, INSERT, VISIT
};

struct Cache_Log {
    enum log_t tp;
    uint32_t id;
};

#endif