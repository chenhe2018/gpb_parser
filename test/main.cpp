#include <iostream>
#include "../message/family.pb.h"
#include "../src/PbPath.h"

int main() {

//#if 0
    std::shared_ptr<Test::Family> family = std::make_shared<Test::Family>();
    Test::Identity* identity = family->mutable_identity();
    identity->set_id(100106);
    identity->set_postcode("201203");
    identity->add_house("Room 201.");
    identity->add_house("NO.3363.");


    ::google::protobuf::int64 i64 {64};
    identity->set_test_int32(32);
    identity->set_test_int64(i64);
    identity->set_test_bool(true);
    identity->set_test_double(3.14);
    identity->set_test_string("test_string");


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
    int ret =0;

    std::string ssss1 {""};
    ret = parser.get_value(static_cast<void*>(family.get()), "identify[1].postcode", ssss1);
    std::cout<<"unique item as repeated:"<<ssss1<<" "<<ret<<std::endl;

    std::string ssss2 {""};
    ret = parser.get_value(static_cast<void*>(family.get()), "person.id", ssss2);
    std::cout<<"repeated item as unique:"<<ssss2<<" "<<ret<<std::endl;

    // basic test
    std::string postcode {""};
    parser.get_value(static_cast<void*>(family.get()), "identity.postcode", postcode);//error at Identity.postcode
    std::cout<<"1.PARSER:"<<postcode<<std::endl;

    std::string surname {""};
    parser.get_value(static_cast<void*>(family.get()), "surname", surname);
    std::cout<<"2.PARSER:"<<surname<<std::endl;


    // out-of-range test
    std::string house {""};
    parser.get_value(static_cast<void*>(family.get()), "identity,house[1]", house);
    std::cout<<"3.PARSER:"<<house<<std::endl;


    std::string house0 {""};
    parser.get_value(static_cast<void*>(family.get()), "identity.house[r]", house0);
    std::cout<<"3.PARSER:"<<house0<<std::endl;


    std::string house2 {""};
    parser.get_value(static_cast<void*>(family.get()), "identity.house[2]", house2);
    std::cout<<"3.PARSER:"<<house2<<std::endl;


    // nesting-repeated test
    std::string email {""};
    parser.get_value(static_cast<void*>(family.get()), "person[0].email[1]", email);
    std::cout<<"4.PARSER:"<<email<<std::endl;

    // nesting-repeated test
    std::string email2 {""};
    parser.get_value(static_cast<void*>(family.get()), "person[0].email[10]", email2);
    std::cout<<"4.PARSER:"<<email2<<std::endl;

    std::string trader_name {""};
    parser.get_value(static_cast<void*>(family.get()), "person[1].trade[1].trade_name", trader_name);
    std::cout<<"5.PARSER:"<<trader_name<<std::endl;

    std::string goods_name {""};
    parser.get_value(static_cast<void*>(family.get()), "person[0),valuable,goods(1].goods_name", goods_name);
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

    // test bool/double/string/int32/int64
    int gret,sret;

    bool test_bool = false, test_bool_new, preb = false;
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_bool", test_bool);
    std::cout<<"=>test_bool: return_value="<<test_bool<<",error_code="<<gret<<std::endl;
    sret = parser.set_value(static_cast<void*>(family.get()), "identity.test_bool", preb);
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_bool", test_bool_new);
    std::cout<<"gret="<<gret<<",sret="<<sret<<",old="<<test_bool<<",new="<<test_bool_new<<std::endl;


    double test_double {0}, test_double_new {0}, pred = 2.718;
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_double", test_double);
    std::cout<<"=>test_double: return_value="<<test_bool<<",error_code="<<gret<<std::endl;
    sret = parser.set_value(static_cast<void*>(family.get()), "identity.test_double", pred);
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_double", test_double_new);
    std::cout<<"gret="<<gret<<",sret="<<sret<<",old="<<test_double<<",new="<<test_double_new<<std::endl;


    std::string test_string {"test_string"}, test_string_new, pres {"new_test_string"};
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_string", test_string);
    std::cout<<"=>test_string: return_value="<<test_string<<",error_code="<<gret<<std::endl;
    sret = parser.set_value(static_cast<void*>(family.get()), "identity.test_string", pres);
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_string", test_string_new);
    std::cout<<"gret="<<gret<<",sret="<<sret<<",old="<<test_string<<",new="<<test_string_new<<std::endl;


    int32 test_int32 {32}, test_int32_new, prei32 {320};
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_int32", test_int32);
    std::cout<<"=>test_int32: return_value="<<test_int32<<",error_code="<<gret<<std::endl;
    sret = parser.set_value(static_cast<void*>(family.get()), "identity.test_int32", prei32);
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_int32", test_int32_new);
    std::cout<<"gret="<<gret<<",sret="<<sret<<",old="<<test_int32<<",new="<<test_int32_new<<std::endl;


    int64 test_int64 {64}, test_int64_new, prei64 {640123456789};
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_int64", test_int64);
    std::cout<<"test_int64: return_value="<<test_int64<<",error_code="<<gret<<std::endl;
    sret = parser.set_value(static_cast<void*>(family.get()), "identity.test_int64", prei64);
    gret = parser.get_value(static_cast<void*>(family.get()), "identity.test_int64", test_int64_new);
    std::cout<<"gret="<<gret<<",sret="<<sret<<",old="<<test_int64<<",new="<<test_int64_new<<std::endl;

    //
    std::string new_postcode {"haha"}, new_postcode_get {""};
    parser.set_value(static_cast<void*>(family.get()), "identity.postcode", new_postcode);//error at Identity.postcode
    parser.get_value(static_cast<void*>(family.get()), "identity.postcode", new_postcode_get);
    std::cout<<"1.PARSER:"<<new_postcode_get<<std::endl;

    //
    int size =0;
    size = parser.size(family.get(),"identity.id");std::cout<<size<<std::endl;
    size = parser.size(family.get(),"identity,id");std::cout<<size<<std::endl;
    size = parser.size(family.get(),"identity.house");std::cout<<size<<std::endl;
    size = parser.size(family.get(),"person[0].valuable.goods");std::cout<<size<<std::endl;
    size = parser.size(family.get(),"person[0].valuable.goods[100]");std::cout<<size<<std::endl;
    size = parser.size(family.get(),"person[0].valuable.goods[0].goods_name");std::cout<<size<<std::endl;
    size = parser.size(family.get(),"person[0].valuable.goods[0].goods_name[0]");std::cout<<size<<std::endl;
    size = parser.size(family.get(),"person[0].valuable.goods[0].goods_name[1]");std::cout<<size<<std::endl;
    size = parser.size(family.get(),"person[0].valuable.goods[0].goods_name[100]");std::cout<<size<<std::endl;

//#endif
    /*
     * summary: error conditions
     * 1.input `item-name` error
     * 2.out of range for repeated
     * 3.wrong cpp_type
     */

    return 0;
}