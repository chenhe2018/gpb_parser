//
// Created by ubuntu on 8/8/18.
//

#ifndef UNTITLED_MSG_PARSER_H
#define UNTITLED_MSG_PARSER_H

#include <string>

typedef long long int64;

class MsgParser
{
public:
    MsgParser(){};
    virtual ~MsgParser(){};
public:
    virtual int get_value(void* p_msg, const std::string & field_name, int & value) = 0;
    virtual int get_value(void* p_msg, const std::string & field_name, double & value) = 0;
    virtual int get_value(void* p_msg, const std::string & field_name, bool & value) = 0;
    virtual int get_value(void* p_msg, const std::string & field_name, std::string & value) = 0;
    virtual int get_value(void* p_msg, const std::string & field_name, int64 & value) = 0;

    virtual int set_value(void* p_msg, const std::string & field_name, int value) = 0;
    virtual int set_value(void* p_msg, const std::string & field_name, double value) = 0;
    virtual int set_value(void* p_msg, const std::string & field_name, bool value) = 0;
    virtual int set_value(void* p_msg, const std::string & field_name, const std::string & value) = 0;
    virtual int set_value(void* p_msg, const std::string & field_name, int64 value) = 0;
};

#endif //UNTITLED_MSG_PARSER_H
