//
// Created by ubuntu on 8/9/18.
//

#ifndef UNTITLED_LOG_H
#define UNTITLED_LOG_H

#include <stdio.h>
#include <memory.h>
#include <time.h>


// 调试日志信息
#define LOG_INFO(format, ...)                                                              \
{                                                                                          \
    time_t t = time(0);                                                                    \
    struct tm ptm;                                                                         \
    memset(&ptm, 0, sizeof(ptm));                                                          \
    localtime_r(&t, &ptm);                                                                 \
    fprintf(stdout, "[ INFO ] [%4d-%02d-%02d %02d:%02d:%02d] [ %s:%s:%d ] " format "\n",   \
            ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour,                  \
            ptm.tm_min, ptm.tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
}

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


//int main()
//{
//
//    LOG_INFO("debug something %d !\n", 1);
//    LOG_ERROR("something error %d !\n", 1);
//    return 0;
//}


#endif //UNTITLED_LOG_H
