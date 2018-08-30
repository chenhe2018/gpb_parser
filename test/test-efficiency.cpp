//
// Created by ubuntu on 8/28/18.
//

#include <iostream>

#include <time.h>

#include "../src/PbPath.h"

#include "../message/testmsg.pb.h"

using namespace std;

/**
  * Test::A *a = base->add_b11();
  * GenA(a);
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

/**
  * Test::C *c = base->add_b12[80]();
  * GenC(c);
    Test::C * c = base->add_b12[80]();
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

#define SERIALIZE(x)    \
({  \
    string encode_str;                  \
    x->SerializeToString(&encode_str);  \
    encode_str;                         \
})  \



void my_tmp_test_reflect(tdf::PbPath parser, Test::base *base, string* strs, int forcount = 1) {
    unsigned long start_time, end_time;
    int ret = 0;
    string tmpstr{""};
    int tmpi32{0};
    int64 tmpi64{0};
    bool tmpbool{true};
    double tmpdouble{1.0};
    std::cout<<base->GetTypeName()<<std::endl;
    // string
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[0], tmpstr);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpstr << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[1], tmpstr);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpstr << std::endl;
    // int32
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[2], tmpi32);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpi32 << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[3], tmpi32);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpi32 << std::endl;
    // int64
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[4], tmpi64);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpi64 << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[5], tmpi64);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpi64 << std::endl;
    // bool
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[6], tmpbool);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpbool << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[7], tmpbool);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpbool << std::endl;
    // double
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[8], tmpdouble);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpdouble << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        ret = parser.get_value(static_cast<void *>(base), strs[9], tmpdouble);
    }
    end_time = GetTickCount();
    std::cout << "ret:" << ret << "\t" << "cost:" << end_time - start_time << "\tres:" << tmpdouble << std::endl;
}


void my_tmp_test_direct(Test::base *base, int forcount = 1) {
    unsigned long start_time, end_time;
    string tmpstr{""};
    int tmpi32{0};
    int64 tmpi64{0};
    bool tmpbool{true};
    double tmpdouble{1.0};
//    string caseBase[] = {"b1","b2[50]","b3","b4[50]","b5","b6[50]","b7","b8[50]","b9","b10[50]"};
//    string caseA[] = {"b11.a1", "b11[50].a2[1]", "b11[50].a3", "b11[50].a4[1]", "b11[50].a5", "b11[50].a6[1]",
//                      "b11[50].a7", "b11[50].a8[1]", "b11[50].a9", "b11[50].a10[1]"};
//    string caseC[] = {"b12[80].cccccccccccccc1", "b12[80].cccccccccccccc2[1]", "b12[80].cccccccccccccc3", "b12[80].cccccccccccccc4[1]",
//                      "b12[80].cccccccccccccc5", "b12[80].cccccccccccccc6[1]", "b12[80].cccccccccccccc7", "b12[80].cccccccccccccc8[1]",
//                      "b12[80].cccccccccccccc9", "b12[80].cccccccccccccc10[1]"};


// 1.base
    std::cout<<"Test::base"<<std::endl;
    //string
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpstr = base->b1();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpstr << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpstr = base->b2(50);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpstr << std::endl;
    //int32
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi32 = base->b3();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi32 << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi32 = base->b4(50);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi32 << std::endl;
    //int64
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi64 = base->b5();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi64 << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi64 = base->b6(50);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi64 << std::endl;
    //bool
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpbool = base->b7();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpbool << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpbool = base->b8(50);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpbool << std::endl;
    //double
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpdouble = base->b9();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpdouble << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpdouble = base->b10(50);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpdouble << std::endl;

// 2.A
    std::cout<<"Test::base::A"<<std::endl;
    //string
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpstr = base->b11(50).a1();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpstr << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpstr = base->b11(50).a2(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpstr << std::endl;
    //int32
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi32 = base->b11(50).a3();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi32 << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi32 = base->b11(50).a4(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi32 << std::endl;
    //int64
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi64 = base->b11(50).a5();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi64 << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi64 = base->b11(50).a6(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi64 << std::endl;
    //bool
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpbool = base->b11(50).a7();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpbool << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpbool = base->b11(50).a8(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpbool << std::endl;
    //double
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpdouble = base->b11(50).a9();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpdouble << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpdouble = base->b11(50).a10(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpdouble << std::endl;

// 3.C
    std::cout<<"Test::base::C"<<std::endl;
    //string
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpstr = base->b12(80).cccccccccccccc1();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpstr << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpstr = base->b12(80).cccccccccccccc2(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpstr << std::endl;
    //int32
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi32 = base->b12(80).cccccccccccccc3();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi32 << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi32 = base->b12(80).cccccccccccccc4(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi32 << std::endl;
    //int64
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi64 = base->b12(80).cccccccccccccc5();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi64 << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpi64 = base->b12(80).cccccccccccccc6(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpi64 << std::endl;
    //bool
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpbool = base->b12(80).cccccccccccccc7();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpbool << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpbool = base->b12(80).cccccccccccccc8(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpbool << std::endl;
    //double
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpdouble = base->b12(80).cccccccccccccc9();
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpdouble << std::endl;
    start_time = GetTickCount();
    for(int i=0;i<forcount;i++){
        tmpdouble = base->b12(80).cccccccccccccc10(1);
    }
    end_time = GetTickCount();
    std::cout << "cost:" << end_time - start_time << "\tres:" << tmpdouble << std::endl;

}


int main() {

    std::shared_ptr<Test::base> base = std::make_shared<Test::base>();
    unsigned long start_time, end_time;
    tdf::PbPath parser;
    int ret = 0;

    // do init
    start_time = GetTickCount();
    int64 i64{64}, i64_2{642};
    // required items
    base->set_b1("b1");
    base->set_b3(3);
    base->set_b5(i64);
    base->set_b7(true);
    base->set_b9(3.14);
    // repeated items
    for(int i=0;i<1000;i++){
        base->add_b2("b2");
        base->add_b4(4);
        base->add_b6(i64_2);
        base->add_b8(false);
        base->add_b10(2.72);
        Test::A *a = base->add_b11();
        GenA(a);
        Test::C *c = base->add_b12();
        GenC(c);
    }
    std::cout << SERIALIZE(base) << std::endl;
    end_time = GetTickCount();
    cout << "time-cost[init]:" << end_time - start_time << endl;

    // benchmark start
    string caseBase[] = {"b1","b2[50]","b3","b4[50]","b5","b6[50]","b7","b8[50]","b9","b10[50]"};
    string caseA[] = {"b11[50].a1", "b11[50].a2[1]", "b11[50].a3", "b11[50].a4[1]", "b11[50].a5", "b11[50].a6[1]",
                      "b11[50].a7", "b11[50].a8[1]", "b11[50].a9", "b11[50].a10[1]"};
    string caseC[] = {"b12[80].cccccccccccccc1", "b12[80].cccccccccccccc2[1]", "b12[80].cccccccccccccc3", "b12[80].cccccccccccccc4[1]",
                      "b12[80].cccccccccccccc5", "b12[80].cccccccccccccc6[1]", "b12[80].cccccccccccccc7", "b12[80].cccccccccccccc8[1]",
                      "b12[80].cccccccccccccc9", "b12[80].cccccccccccccc10[1]"};
    
    // tmp test
    std::cout<<"**** reflect ****"<<std::endl;
    my_tmp_test_reflect(parser,base.get(),caseBase);
    my_tmp_test_reflect(parser,base.get(),caseA,100000);
    my_tmp_test_reflect(parser,base.get(),caseC,100000);

    std::cout<<"**** direct ****"<<std::endl;
    my_tmp_test_direct(base.get(),100000);
/*
    // print
    start_time = GetTickCount();
    std::string encode_str;
    base->SerializeToString(&encode_str);
    std::cout << encode_str << std::endl;
    end_time = GetTickCount();
    cout << "time cost:" << end_time - start_time << endl;
    */

    return 0;
}