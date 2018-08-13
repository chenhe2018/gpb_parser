#include <iostream>
#include "family.pb.h"
#include "PbPath.h"

int main() {

//#if 0
    std::shared_ptr<Test::Family> family = std::make_shared<Test::Family>();
    Test::Identity* identity = family->mutable_identity();
    identity->set_id(100106);
    identity->set_postcode("201203");
    identity->add_house("Room 201.");
    identity->add_house("NO.3363.");
    identity->set_is_teenager(true);
    identity->set_test1(3.1415926);
    ::google::protobuf::int64 i64 {64};
    identity->set_test2(i64);


    Test::Person* person1 = family->add_person();
    person1->set_id(1);
    person1->set_name("son");
    person1->add_email("18@grow"); person1->add_email("18@up");
    // repeated Trade
    Test::Trade* trade1 = person1->add_trade();
    trade1->set_trade_name("salary"); trade1->set_trade_value(1000);
    Test::Trade* trade2 = person1->add_trade();
    trade2->set_trade_name("cost"); trade2->set_trade_value(-5000);
    // required Valuable
    Test::Valuable* valuable = person1->mutable_valuable();
    Test::Goods* goods1 = valuable->add_goods();
    goods1->set_goods_name("iphone"); goods1->set_goods_value(5000);
    Test::Goods* goods2 = valuable->add_goods();
    goods2->set_goods_name("pc"); goods2->set_goods_value(4000);


    Test::Person* person2 = family->add_person();
    person2->set_id(2);
    person2->set_name("mother");
    person2->add_email("48@old");
    // repeated Trade
    Test::Trade* trade10 = person2->add_trade();
    trade10->set_trade_name("salary"); trade10->set_trade_value(4000);
    Test::Trade* trade20 = person2->add_trade();
    trade20->set_trade_name("salary"); trade20->set_trade_value(4000);
    // required Valuable
    Test::Valuable* valuable0 = person2->mutable_valuable();
    Test::Goods* goods10 = valuable0->add_goods();
    goods10->set_goods_name("miphone"); goods10->set_goods_value(2000);


    family->set_surname("CFETS");

    // test
    std::string encode_str;
    family->SerializeToString(&encode_str);
    std::cout<<encode_str<<std::endl;


    // parser test start here
    std::cout<<"-----------test gpb parser------------"<<std::endl;
    tdf::PbPath parser;

    // basic test
    std::string postcode {""};
    parser.get_value(static_cast<void*>(family.get()), "identity.postcode", postcode);//error at Identity.postcode
    std::cout<<"1.PARSER:"<<postcode<<std::endl;

    std::string surname {""};
    parser.get_value(static_cast<void*>(family.get()), "surname", surname);
    std::cout<<"2.PARSER:"<<surname<<std::endl;


    // out-of-range test
    std::string house {""};
    parser.get_value(static_cast<void*>(family.get()), "identity.house[1]", house);
    std::cout<<"3.PARSER:"<<house<<std::endl;


    std::string house0 {""};
    parser.get_value(static_cast<void*>(family.get()), "identity.house[0]", house0);
    std::cout<<"3.PARSER:"<<house0<<std::endl;


    std::string house2 {""};
    parser.get_value(static_cast<void*>(family.get()), "identity.house[2]", house2);
    std::cout<<"3.PARSER:"<<house2<<std::endl;


    // nesting-repeated test
    std::string email {""};
    parser.get_value(static_cast<void*>(family.get()), "person[0].email[1]", email);
    std::cout<<"4.PARSER:"<<email<<std::endl;

    std::string trader_name {""};
    parser.get_value(static_cast<void*>(family.get()), "person[1].trade[1].trade_name", trader_name);
    std::cout<<"5.PARSER:"<<trader_name<<std::endl;

    std::string goods_name {""};
    parser.get_value(static_cast<void*>(family.get()), "person[0].valuable.goods[1].goods_name", goods_name);
    std::cout<<"6.PARSER:"<<goods_name<<std::endl;


    // error test
    std::string error_goods_name1 {""};
    int error1 = parser.get_value(static_cast<void*>(family.get()), "person[0].valuable.goods[7000].goods_name", error_goods_name1);//out of range
    std::cout<<"1.Error: return_value="<<error_goods_name1<<",error_code="<<error1<<std::endl;


    std::string error_goods_name2 {""};
    int error2 = parser.get_value(static_cast<void*>(family.get()), "person[0].valuable.Goods[1].goods_name", error_goods_name2);//wrong item
    std::cout<<"2.Error: return_value="<<error_goods_name2<<",error_code="<<error2<<std::endl;


    std::string error_goods_name3 {""};
    int error3 = parser.get_value(static_cast<void*>(family.get()), "person[0].valuable.goods[1).goods_name", error_goods_name3);//wrong format
    std::cout<<"3.Error: return_value="<<error_goods_name3<<",error_code="<<error3<<std::endl;


    int error_goods_name4 {0};
    int error4 = parser.get_value(static_cast<void*>(family.get()), "person[0].valuable.goods[1].goods_name", error_goods_name4);//wrong cpptype
    std::cout<<"4.Error: return_value="<<error_goods_name4<<",error_code="<<error4<<std::endl;


    std::string* error_goods_name5 = nullptr;
    int error5 = parser.get_value(static_cast<void*>(family.get()), "person[0].valuable.goods[1].goods_name", *error_goods_name5);//null point input
    std::cout<<"5.Error: return_value="<<error_goods_name5<<",error_code="<<error5<<std::endl;

    // test bool/double/int64
    bool is_teenager = false, is_teenager_new;
    int testbool = parser.get_value(static_cast<void*>(family.get()), "identity.is_teenager", is_teenager);
    std::cout<<"1.bool: return_value="<<is_teenager<<",error_code="<<testbool<<std::endl;
    int retbool = parser.set_value(static_cast<void*>(family.get()), "identity.is_teenager", false);
    parser.get_value(static_cast<void*>(family.get()), "identity.is_teenager", is_teenager_new);
    std::cout<<"IsSucess="<<retbool<<",old="<<is_teenager<<",new="<<is_teenager_new<<std::endl;

    double test1 {0}, test1_new {0};
    int testdouble = parser.get_value(static_cast<void*>(family.get()), "identity.test1", test1);
    std::cout<<"2.double: return_value="<<is_teenager<<",error_code="<<testdouble<<std::endl;
    int retdouble = parser.set_value(static_cast<void*>(family.get()), "identity.test1", 3.14);
    parser.get_value(static_cast<void*>(family.get()), "identity.test1", test1_new);
    std::cout<<"IsSucess="<<retdouble<<",old="<<test1<<",new="<<test1_new<<std::endl;

    int64 test2 {0}, test2_new {0}, test2_new_in {123123123};
    int testint64 = parser.get_value(static_cast<void*>(family.get()), "identity.test2", test2);
    std::cout<<"3.bool: return_value="<<is_teenager<<",error_code="<<testint64<<std::endl;
    int retint64 = parser.set_value(static_cast<void*>(family.get()), "identity.test2", test2_new_in);
    parser.get_value(static_cast<void*>(family.get()), "identity.test2", test2_new);
    std::cout<<"IsSucess="<<retint64<<",old="<<test2<<",new="<<test2_new<<std::endl;

    std::string new_postcode {"haha"}, new_postcode_get {""};
    parser.set_value(static_cast<void*>(family.get()), "identity.postcode", new_postcode);//error at Identity.postcode
    parser.get_value(static_cast<void*>(family.get()), "identity.postcode", new_postcode_get);
    std::cout<<"1.PARSER:"<<new_postcode_get<<std::endl;

//#endif
    /*
     * summary: error conditions
     * 1.input `item-name` error
     * 2.out of range for repeated
     * 3.wrong cpp_type
     */

    return 0;
}