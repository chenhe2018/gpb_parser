//
// Created by ubuntu on 8/8/18.
//

#include "PbPath.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/reflection.h"
#include <iostream>
#include <vector>

#include "LOG.h"

namespace string_util {
    std::vector<std::string> split(const std::string &input, char delimeter) {
        std::stringstream ss{input};
        std::string item;
        std::vector<std::string> split_strings;
        while (std::getline(ss, item, delimeter)) {
            split_strings.push_back(item);
        }
        return split_strings;
    }

    std::string trim(const std::string &input) {
        std::string output{input};
        if (input.empty()) return output;
        output.erase(0, output.find_first_not_of(" "));
        output.erase(output.find_last_not_of(" ") + 1);
        return output;
    }

    bool symbol_check(char c) {
        if (c >= 'a' && c <= 'z')
            return true;
        if (c >= 'A' && c <= 'Z')
            return true;
        if (c >= '0' && c <= '9')
            return true;
        if (c == '_')
            return true;
        if (c == '.')
            return true;
        if (c == '[' || c == ']')
            return true;
        return false;
    }

    /*
     * ret: 0-n, shift
     *       -1, no shift
     *       -2, format error
     *       -3, stoi error
     */
    int format(std::string &expression) {
//        for (unsigned int i = 0; i < expression.size(); i++) {
//            if (!symbol_check(expression.at(i)))
//                return -2;
//        }
        int left = expression.find('[');
        int right = expression.find(']');
        if (left < 0) {
            return -1;
        } else if (left > 0 && right > 0 && right > left) {
            try{
                int shift = std::stoi(expression.substr(left + 1, right - left + 1));
                expression = expression.substr(0, left);
                return shift;
            }catch (...){
                // stoi error
                return -3;
            }
        } else {
            // format error
            return -2;
        }
    }
}


namespace tdf {

    using ::google::protobuf::Reflection;
    using ::google::protobuf::Message;
    using ::google::protobuf::FieldDescriptor;
    using ::google::protobuf::Descriptor;

    PbPath::PbPath() {}

    PbPath::~PbPath() {}

    reflection_tuple_t PbPath::get_field_descriptor(void *p_msg, const std::string &field_name) {
        Message *message{static_cast<Message *>(p_msg)};
        Descriptor *descriptor{const_cast<Descriptor *>(message->GetDescriptor())};
        Reflection *reflection{const_cast<Reflection *>(message->GetReflection())};
        FieldDescriptor *fieldDescriptor{nullptr};
        std::vector<std::string> path_sections{string_util::split(field_name, '.')};
        /* SHITF: 0-n, repeated item
         *         -1, required or optional item
         *         -2, error because wrong item_name
         *         -3, error because out of range
         *         -4, error because wrong expression_format
         *         -5, error because stoi
         */
        int SHIFT = -1;
        for (auto section: path_sections) {
            int shift = string_util::format(section);
            if (shift >= 0) {
                //Repeated
                SHIFT = shift;
                // reflection here
                fieldDescriptor = const_cast<FieldDescriptor *>(descriptor->FindFieldByName(section));
                if (fieldDescriptor == nullptr) {
                    SHIFT = -2;
                    LOG_ERROR("wrong item_name, %s[%d]", section.c_str(), shift);
                    break;
                }
                if (!fieldDescriptor->is_repeated()) {
                    // unique items treated as repeated
                    SHIFT = -3;
                    LOG_ERROR("unique items treated as repeated, %s[%d]", section.c_str(), shift);
                    break;
                }
                reflection = const_cast<Reflection *>(message->GetReflection());
                int size = reflection->FieldSize(*message, fieldDescriptor);
                if (shift <= size - 1 && size >= 0) {
                    // sucess
                    if (fieldDescriptor->type() == FieldDescriptor::TYPE_MESSAGE && fieldDescriptor->is_repeated()) {
                        message = reflection->MutableRepeatedMessage(message, fieldDescriptor, shift);
                        descriptor = const_cast<Descriptor *>(message->GetDescriptor());
                    }
                } else {
                    // out of range
                    SHIFT = -3;
                    LOG_ERROR("out of range, %s[%d]", section.c_str(), shift);
                    break;
                }
            } else if (shift == -1) {
                SHIFT = -1;
                //Normal
                fieldDescriptor = const_cast<FieldDescriptor *>(descriptor->FindFieldByName(section));
                if (fieldDescriptor == nullptr) {
                    SHIFT = -2;
                    LOG_ERROR("wrong item_name, %s", section.c_str());
                    break;
                }
                if (fieldDescriptor->is_repeated()) {
                    // repeated items treated as unique
                    SHIFT = -3;
                    LOG_ERROR("repeated items treated as unique, %s[%d]", section.c_str(), shift);
                    break;
                }
                reflection = const_cast<Reflection *>(message->GetReflection());
                // sucess
                if (fieldDescriptor->type() == FieldDescriptor::TYPE_MESSAGE) {
                    message = reflection->MutableMessage(message, fieldDescriptor);
                    if (message == nullptr) {
                        SHIFT = -2;
                        LOG_ERROR("wrong item_name, %s", section.c_str());
                        break;
                    }
                    descriptor = const_cast<Descriptor *>(message->GetDescriptor());
                }
            } else if (shift == -2) {
                // error
                SHIFT = -4;
                LOG_ERROR("format error, %s", section.c_str());
                break;
            } else if (shift == -3) {
                // error
                SHIFT = -5;
                LOG_ERROR("stoi error, %s", section.c_str());
                break;
            } else {
                // error
                SHIFT = -5;
                LOG_ERROR("stoi error, %s", section.c_str());
                break;
            }
        }
        return std::make_tuple(reflection, message, fieldDescriptor, SHIFT);
    }

    int PbPath::get_value(void *p_msg, const std::string &field_name, int &value) {
        if (nullptr == p_msg || nullptr == &field_name || nullptr == &value) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_INT32) {
            value = (shift >= 0) ? reflection->GetRepeatedInt32(*message, fieldDescriptor, shift)
                                 : reflection->GetInt32(*message, fieldDescriptor);
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            LOG_ERROR("wrong cpptype");
//            value = 0;
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::get_value(void *p_msg, const std::string &field_name, double &value) {
        if (nullptr == p_msg || nullptr == &field_name || nullptr == &value) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_DOUBLE) {
            value = (shift >= 0) ? reflection->GetRepeatedDouble(*message, fieldDescriptor, shift)
                                 : reflection->GetDouble(*message, fieldDescriptor);
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            LOG_ERROR("wrong cpptype");
//            value = 0;
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::get_value(void *p_msg, const std::string &field_name, bool &value) {
        if (nullptr == p_msg || nullptr == &field_name || nullptr == &value) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_BOOL) {
            value = (shift >= 0) ? reflection->GetRepeatedBool(*message, fieldDescriptor, shift)
                                 : reflection->GetBool(*message, fieldDescriptor);
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            LOG_ERROR("wrong cpptype");
            //value = false;
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::get_value(void *p_msg, const std::string &field_name, std::string &value) {
        if (nullptr == p_msg || nullptr == &field_name || nullptr == &value) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        /* SHITF: 0-n, repeated item
         *         -1, required or optional item
         *         -2, error because wrong item_name
         *         -3, error because out of range
         *         -4, error because wrong expression_format
         */
        switch (shift) {
            case -2:
//                value = "";
                return RESULT::ERROR_NAME;
            case -3:
//                value = "";
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
//                value = "";
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_STRING) {
            value = (shift >= 0) ? reflection->GetRepeatedString(*message, fieldDescriptor, shift)
                                 : reflection->GetString(*message, fieldDescriptor);
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
//            value = "";
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::get_value(void *p_msg, const std::string &field_name, int64 &value) {
        if (nullptr == p_msg || nullptr == &field_name || nullptr == &value) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_INT64) {
            value = (shift >= 0) ? reflection->GetRepeatedInt64(*message, fieldDescriptor, shift)
                                 : reflection->GetInt64(*message, fieldDescriptor);
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            LOG_ERROR("wrong cpptype");
//            value = 0;
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::set_value(void *p_msg, const std::string &field_name, int value) {
        if (nullptr == p_msg || nullptr == &field_name) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_INT32) {
            if (shift >= 0) {
                reflection->SetRepeatedInt32(message, fieldDescriptor, shift, static_cast<google::protobuf::int32>(value));
            } else{
                reflection->SetInt32(message, fieldDescriptor,static_cast<google::protobuf::int32>(value));
            }
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            LOG_ERROR("wrong cpptype");
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::set_value(void *p_msg, const std::string &field_name, double value) {
        if (nullptr == p_msg || nullptr == &field_name) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_DOUBLE) {
            if (shift >= 0) {
                reflection->SetRepeatedDouble(message, fieldDescriptor, shift, value);
            }else{
                reflection->SetDouble(message, fieldDescriptor, value);
            }
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            LOG_ERROR("wrong cpptype");
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::set_value(void *p_msg, const std::string &field_name, bool value) {
        if (nullptr == p_msg || nullptr == &field_name) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_BOOL) {
            if(shift >= 0) {
                reflection->SetRepeatedBool(message, fieldDescriptor, shift, value);
            }else{
                reflection->SetBool(message, fieldDescriptor, value);
            }
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            LOG_ERROR("wrong cpptype");
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::set_value(void *p_msg, const std::string &field_name, const std::string &value) {
        if (nullptr == p_msg || nullptr == &field_name || nullptr == &value) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_STRING) {
            if (shift >= 0) {
                reflection->SetRepeatedString(message, fieldDescriptor, shift, value);
            }else{
                reflection->SetString(message, fieldDescriptor, value);
            }
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::set_value(void *p_msg, const std::string &field_name, int64 value) {
        if (nullptr == p_msg || nullptr == &field_name) {
            LOG_ERROR("input nullptr");
            return RESULT::ERROR_NULL_POINT;
        }
        reflection_tuple_t reflection_tuple{get_field_descriptor(p_msg, field_name)};
        Reflection *reflection{std::get<0>(reflection_tuple)};
        Message *message{std::get<1>(reflection_tuple)};
        FieldDescriptor *fieldDescriptor{std::get<2>(reflection_tuple)};
        int shift{std::get<3>(reflection_tuple)};
        switch (shift) {
            case -2:
                return RESULT::ERROR_NAME;
            case -3:
                return RESULT::ERROR_OUTOFRANGE;
            case -4:
                return RESULT::ERROR_FORMAT;
            case -5:
                return RESULT::ERROR_STOI;
            default:
                break;
        }
        if (reflection == nullptr && message == nullptr && fieldDescriptor == nullptr) {
            return RESULT::WARN_SYSTEMERROR;
        }
        // do
        FieldDescriptor::CppType type = fieldDescriptor->cpp_type();
        if (type == FieldDescriptor::CPPTYPE_INT64) {
            if (shift >= 0){
                reflection->SetRepeatedInt64(message, fieldDescriptor, shift, value);
            }else{
                reflection->SetInt64(message, fieldDescriptor, value);
            }
            return RESULT::SUCCESS;
        } else {
            // error, wrong CPPTYPE
            LOG_ERROR("wrong cpptype");
//            value = 0;
            return RESULT::WARN_WRONG_CPPTYPE;
        }
    }

    int PbPath::size(void *p_msg, const std::string &field_name) {
        Message *message{static_cast<Message *>(p_msg)};
        Descriptor *descriptor{const_cast<Descriptor *>(message->GetDescriptor())};
        Reflection *reflection{const_cast<Reflection *>(message->GetReflection())};
        FieldDescriptor *fieldDescriptor{nullptr};
        std::vector<std::string> path_sections{string_util::split(field_name, '.')};
        for (unsigned int i = 0; i < path_sections.size() - 1; i++) {
            std::string section = path_sections[i];
            int shift = string_util::format(section);
            if (shift >= 0) {
                //Repeated
                // reflection here
                fieldDescriptor = const_cast<FieldDescriptor *>(descriptor->FindFieldByName(section));
                if (fieldDescriptor == nullptr) {
                    LOG_ERROR("wrong item_name, %s[%d]", section.c_str(), shift);
                    return RESULT::ERROR_NAME;
                }
                reflection = const_cast<Reflection *>(message->GetReflection());
                int size = reflection->FieldSize(*message, fieldDescriptor);
                if (shift <= size - 1 && size >= 0) {
                    // sucess
                    if (fieldDescriptor->type() == FieldDescriptor::TYPE_MESSAGE && fieldDescriptor->is_repeated()) {
                        message = reflection->MutableRepeatedMessage(message, fieldDescriptor, shift);
                        descriptor = const_cast<Descriptor *>(message->GetDescriptor());
                    }
                } else {
                    // out of range
                    LOG_ERROR("out of range, %s[%d]", section.c_str(), shift);
                    return RESULT::ERROR_OUTOFRANGE;
                }
            } else if (shift == -1) {
                //Normal
                fieldDescriptor = const_cast<FieldDescriptor *>(descriptor->FindFieldByName(section));
                if (fieldDescriptor == nullptr) {
                    LOG_ERROR("wrong item_name, %s", section.c_str());
                    return RESULT::ERROR_NAME;
                }
                reflection = const_cast<Reflection *>(message->GetReflection());
                if (fieldDescriptor->type() == FieldDescriptor::TYPE_MESSAGE) {
                    message = reflection->MutableMessage(message, fieldDescriptor);
                    if (message == nullptr) {
                        LOG_ERROR("wrong item_name, %s", section.c_str());
                        return RESULT::ERROR_NAME;
                    }
                    descriptor = const_cast<Descriptor *>(message->GetDescriptor());
                }
            } else {
                // error
                LOG_ERROR("format error, %s", section.c_str());
                return RESULT::ERROR_FORMAT;
            }
        }
        // last item
        std::string section = path_sections[path_sections.size() - 1];
        int shift = string_util::format(section);
        fieldDescriptor = const_cast<FieldDescriptor *>(descriptor->FindFieldByName(section));
        if (fieldDescriptor == nullptr) {
            LOG_ERROR("wrong item_name, %s", section.c_str());
            return RESULT::ERROR_NAME;
        }
        reflection = const_cast<Reflection *>(message->GetReflection());
        if (shift >= 0) {
            // last item format error
            // we think the last item cannot have [], e.g., person[0].valuable.goods[100]
            LOG_INFO("last item format error, %s",section.c_str());
            return RESULT::ERROR_FORMAT;
        }
        return fieldDescriptor->is_repeated() ? reflection->FieldSize(*message, fieldDescriptor) : 1;
    }


}
