//
// Created by ubuntu on 8/28/18.
//

#include <iostream>

#include <time.h>

#include "../src/PbPath.h"

#include "../message/testmsg.pb.h"

using namespace std;

/*
    Test::A * a = base->add_b11();
    a->set_a1("a1");
    a->add_a2("a2_0");a->add_a2("a2_1");
    a->set_a3(1);
    a->add_a4(5);a->add_a4(7);a->add_a4(9);
    ::google::protobuf::int64 i64[3] {2,1024,2048};
    a->set_a5(i64[0]);
    a->add_a6(i64[1]);a->add_a6(i64[2]);
    a->set_a7(true);
    a->add_a8(false);a->add_a8(false);a->add_a8(false);
    a->set_a9(3.1415926);
    a->add_a10(1.0);a->add_a10(2.0);a->add_a10(3.0);
*/
#define GenA(a){    \
    a->set_a1("a1");                                        \
    a->add_a2("a2_0");a->add_a2("a2_1");                    \
    a->set_a3(1);                                           \
    a->add_a4(5);a->add_a4(7);a->add_a4(9);                 \
    ::google::protobuf::int64 i64[3] {2,1024,2048};         \
    a->set_a5(i64[0]);                                      \
    a->add_a6(i64[1]);a->add_a6(i64[2]);                    \
    a->set_a7(true);                                        \
    a->add_a8(false);a->add_a8(false);a->add_a8(false);     \
    a->set_a9(3.1415926);                                   \
    a->add_a10(1.0);a->add_a10(2.0);a->add_a10(3.0);        \
    a;                                                      \
}   \

/*
    Test::C * c = base->add_b12();
    c->set_cccccccccccccc1("c1");
    c->add_cccccccccccccc2("c2_0");c->add_cccccccccccccc2("c2_1");
    c->set_cccccccccccccc3(100);
    c->add_cccccccccccccc4(500);c->add_cccccccccccccc4(700);c->add_cccccccccccccc4(900);
    ::google::protobuf::int64 i64[3] {5, -1024, -2048};
    c->set_cccccccccccccc5(i64[0]);
    c->add_cccccccccccccc6(i64[1]);c->add_cccccccccccccc6(i64[2]);
    c->set_cccccccccccccc7(true);
    c->add_cccccccccccccc8(false);c->add_cccccccccccccc8(false);c->add_cccccccccccccc8(false);
    c->set_cccccccccccccc9(2.71828);
    c->add_cccccccccccccc10(-1.0);c->add_cccccccccccccc10(-2.0);c->add_cccccccccccccc10(-3.0);
 */
#define GenC(c){   \
    c->set_cccccccccccccc1("c1");                                                                   \
    c->add_cccccccccccccc2("c2_0");c->add_cccccccccccccc2("c2_1");                                  \
    c->set_cccccccccccccc3(100);                                                                    \
    c->add_cccccccccccccc4(500);c->add_cccccccccccccc4(700);c->add_cccccccccccccc4(900);            \
    ::google::protobuf::int64 i64[3] {5, -1024, -2048};                                             \
    c->set_cccccccccccccc5(i64[0]);                                                                 \
    c->add_cccccccccccccc6(i64[1]);c->add_cccccccccccccc6(i64[2]);                                  \
    c->set_cccccccccccccc7(true);                                                                   \
    c->add_cccccccccccccc8(false);c->add_cccccccccccccc8(false);c->add_cccccccccccccc8(false);      \
    c->set_cccccccccccccc9(2.71828);                                                                \
    c->add_cccccccccccccc10(-1.0);c->add_cccccccccccccc10(-2.0);c->add_cccccccccccccc10(-3.0);      \
    c;                                                                                              \
}   \

/**
 * us
 * (1).time_t time(time_t *timer);
 * 返回以格林尼治时间（GMT）为标准，从1970年1月1日00:00:00到现在的此时此刻所经过的秒数
 * (2).clock_t clock(void);
 * 返回进程启动到调用函数时所经过的CPU时钟计时单元（clock tick）数，在MSDN中称之为挂钟时间（wal-clock），以毫秒为单位
 */
#define GetTickCount()  \
({  \
    unsigned long ret;                              \
    struct timespec ts;                             \
    clock_gettime(CLOCK_MONOTONIC, &ts);            \
    ret = ts.tv_sec * 1000 + ts.tv_nsec/(1000*1000);\
    ret;                                            \
})  \

//#include <unistd.h>
//    sleep(1);
//    usleep(1000);

int main() {

    std::shared_ptr<Test::base> base = std::make_shared<Test::base>();
    unsigned long start_time, end_time, subtime;

    // do
    start_time = GetTickCount();

    Test::A *a = base->add_b11();
    GenA(a);
    for(int i=0;i<100;i++){
        Test::A *ai = base->add_b11();
        GenA(ai);
    }
//    Test::C *c = base->add_b12();
//    GenC(c);

    end_time = GetTickCount();
    cout << "time cost:" << end_time - start_time << endl;

    // print
    start_time = GetTickCount();
    std::string encode_str;
    base->SerializeToString(&encode_str);
    //std::cout << encode_str << std::endl;
    end_time = GetTickCount();
    cout << "time cost:" << end_time - start_time << endl;

    return 0;
}