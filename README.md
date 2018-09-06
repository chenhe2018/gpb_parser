# pbpath库

pbpath库支持对GPB对象的路径化反射查询，支持重复组。例如，支持形如"person[1].valuable.good[1]"的set、get功能。

Function:

(1)get\set value through path.

(2)get repeated size through path.

## 编译执行

>mkdir build && cd build

>cmake .. && make

## 测试使用

本工程提供了基本测试用例可供参考。

>./test/main.cpp

Compile your test with `libpbpath.so` and `*.h`. 

>g++ -o test main.cpp family.pb.cc -std=c++11 -Wall -g -I`pwd`/protobuf/include -L`pwd`/protobuf/lib -lprotobuf -I`pwd` -L`pwd` -lpbpath

## 参考

### 1.cmake

Compile project with these two lines.


### 2.adjust catalog

Subdirectory added in protoc compile command as follow.

>protoc -I=./message/ --cpp_out=./message/ ./message/*.proto

### 3.cpack

CPack after cmake.

>cpack --config CPackConfig.cmake

### 4.macro triger

``` C++
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    add_definitions(-D DEBUG)
endif()
```

1)build.sh or build_release.sh

>cmake ..

>cmake -DCMAKE_BUILD_TYPE=Release ..

2)'-D DEBUG' controls LOG.h

```C++
#ifdef DEBUG
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
#else
#define LOG_INFO(format, ...)
#endif
```
 



