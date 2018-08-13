//
// Created by ubuntu on 8/8/18.
//
#pragma once

#ifndef UNTITLED_GPB_MSG_PARSER_H
#define UNTITLED_GPB_MSG_PARSER_H

//#include "unused/msg_parser.h"
#include <tuple>
#include <vector>
#include <sstream>

typedef long long int64;

namespace google {
    namespace protobuf {
        class Reflection;

        class Message;

        class FieldDescriptor;

        class Descriptor;
    }
}
/*
namespace string_util{
    std::vector<std::string> split(const std::string & input, char delimeter)
    {
        std::stringstream ss {input};
        std::string item;
        std::vector<std::string> split_strings;
        while (std::getline(ss, item, delimeter))
        {
            split_strings.push_back(item);
        }
        return split_strings;
    }

    std::string trim(const std::string& input)
    {
        std::string output{input};
        if(input.empty()) return output;
        output.erase(0, output.find_first_not_of(" "));
        output.erase(output.find_last_not_of(" ")+1);
        return output;
    }
}
 */
namespace string_util {
    std::vector<std::string> split(const std::string &input, char delimeter);

    std::string trim(const std::string &input);

    /*
     * ret: 0-n, shift
     *       -1, no shift
     *       -2, format error
     */
    int format(std::string &expression);
}

namespace tdf {
    using reflection_tuple_t = std::tuple<::google::protobuf::Reflection *, ::google::protobuf::Message *, ::google::protobuf::FieldDescriptor *, int>;

    class PbPath {
    public:
        PbPath();

        virtual ~PbPath();

        virtual int get_value(void *p_msg, const std::string &field_name, int &value);

        virtual int get_value(void *p_msg, const std::string &field_name, double &value);

        virtual int get_value(void *p_msg, const std::string &field_name, bool &value);

        virtual int get_value(void *p_msg, const std::string &field_name, std::string &value);

        virtual int get_value(void *p_msg, const std::string &field_name, int64 &value);

        virtual int set_value(void *p_msg, const std::string &field_name, int value);

        virtual int set_value(void *p_msg, const std::string &field_name, double value);

        virtual int set_value(void *p_msg, const std::string &field_name, bool value);

        virtual int set_value(void *p_msg, const std::string &field_name, const std::string &value);

        virtual int set_value(void *p_msg, const std::string &field_name, int64 value);

        enum RESULT {
            SUCCESS = 0,
            ERROR_NAME = -1,
            ERROR_FORMAT = -2,
            ERROR_OUTOFRANGE = -3,
            WARN_SYSTEMERROR = -4,
            WARN_WRONG_CPPTYPE = -5,
            ERROR_NULL_POINT = -6
        };

    private:
        reflection_tuple_t get_field_descriptor(void *p_msg, const std::string &field_name);
    };
}

#ifdef __cplusplus
extern "C" {
#endif
tdf::PbPath *create();
#ifdef __cplusplus
};
#endif


#endif //UNTITLED_GPB_MSG_PARSER_H
