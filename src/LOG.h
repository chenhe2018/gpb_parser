#ifndef UNTITLED_LOG_H
#define UNTITLED_LOG_H

#include <stdio.h>
#include <memory.h>
#include <time.h>

// -D DEBUG, active
// add_definitions(-D DEBUG)
#ifdef DEBUG

// 调试日志信息
#define LOG_INFO(format, ...)                                                              \
({                                                                                         \
    time_t t = time(0);                                                                    \
    struct tm ptm;                                                                         \
    memset(&ptm, 0, sizeof(ptm));                                                          \
    localtime_r(&t, &ptm);                                                                 \
    fprintf(stdout, "[ INFO ] [%4d-%02d-%02d %02d:%02d:%02d] [ %s:%s:%d ] " format "\n",   \
            ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour,                  \
            ptm.tm_min, ptm.tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
})

// 错误日志信息
#define LOG_ERROR(format, ...)                                                             \
{                                                                                          \
    time_t t = time(0);                                                                    \
    struct tm ptm;                                                                         \
    memset(&ptm, 0, sizeof(ptm));                                                          \
    localtime_r(&t, &ptm);                                                                 \
    fprintf(stderr, "[ ERROR ] [%4d-%02d-%02d %02d:%02d:%02d] [ %s:%s:%d ] " format "\n",  \
            ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour,                  \
            ptm.tm_min, ptm.tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
}

#else
#define LOG_INFO(format, ...)
#define LOG_ERROR(format, ...)
#endif


#endif //UNTITLED_LOG_H
