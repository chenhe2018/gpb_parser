#pragma once

#ifndef UNTITLED_GPB_MSG_PARSER_H
#define UNTITLED_GPB_MSG_PARSER_H

#include <tuple>
#include <vector>
#include <sstream>
#include <google/protobuf/stubs/port.h>

//typedef long long int64;

using ::google::protobuf::int32;
using ::google::protobuf::int64;

namespace google {
    namespace protobuf {
        class Reflection;
        class Message;
        class FieldDescriptor;
        class Descriptor;
    }
}

namespace string_util {
    std::vector<std::string> split(const std::string &input, char delimeter);

    std::string trim(const std::string &input);

    bool symbol_check(char c);

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

        virtual int get_value(void *p_msg, const std::string &field_name, int32 &value);

        virtual int get_value(void *p_msg, const std::string &field_name, double &value);

        virtual int get_value(void *p_msg, const std::string &field_name, bool &value);

        virtual int get_value(void *p_msg, const std::string &field_name, std::string &value);

        virtual int get_value(void *p_msg, const std::string &field_name, int64 &value);

        virtual int set_value(void *p_msg, const std::string &field_name, int32 value);

        virtual int set_value(void *p_msg, const std::string &field_name, double value);

        virtual int set_value(void *p_msg, const std::string &field_name, bool value);

        virtual int set_value(void *p_msg, const std::string &field_name, const std::string &value);

        virtual int set_value(void *p_msg, const std::string &field_name, int64 value);

        /* Func: get size of repeated\unique item
         * get_size(family.person)          ->3
         * get_size(family.person[2].id)    ->1
         */
        virtual int size(void *p_msg, const std::string &field_name);

        enum RESULT {
            SUCCESS = 0,
            ERROR_NAME = -1,
            ERROR_FORMAT = -2,
            ERROR_OUTOFRANGE = -3,
            WARN_SYSTEMERROR = -4,
            WARN_WRONG_CPPTYPE = -5,
            ERROR_NULL_POINT = -6,
            ERROR_STOI = -7
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
