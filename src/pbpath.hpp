#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>

namespace cfets_protobuf {

    class StringUtils {
    public:
        std::string trim(std::string &s);

        void split(std::string &srcString, char delim, std::vector<std::string> &stringSets);
    };

    std::string StringUtils::trim(std::string &s) {
        if (s.empty()) return s;
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
        return s;
    }

    void StringUtils::split(std::string &srcString, char delim, std::vector<std::string> &stringSets) {
        std::string fieldNameStr = trim(srcString);
        int i = 0;
        auto pos = fieldNameStr.find(delim);
        if (pos == std::string::npos) {
            stringSets.push_back(fieldNameStr);
            return;
        }
        while (pos != std::string::npos) {
            stringSets.push_back(fieldNameStr.substr(i, pos - i));
            i = ++pos;
            pos = fieldNameStr.find(delim, pos);
            if (pos == std::string::npos) {
                stringSets.push_back(fieldNameStr.substr(i, fieldNameStr.length()));
            }
        }
    }

    class pbpath {
    public:
        pbpath(const std::string &name, const char *binaryData, const int length)
                : messageName(name) {
            messageContent = std::string(binaryData, length);
        }

        bool ParseBinary();

        int GetFieldValue(std::string &fieldName, std::string &result);

        int GetFieldValue(std::string &fieldName, double &result);

        int GetFieldValue(std::string &fieldName, float &result);

        int GetFieldValue(std::string &fieldName, bool &result);

        int GetFieldValue(std::string &fieldName, int &result);

        int GetFieldValue(std::string &fieldName, long long &result);

        int GetFieldValue(std::string &fieldName, unsigned int &result);

        int GetFieldValue(std::string &fieldName, unsigned long long &result);

    private:
        bool IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                             const google::protobuf::Descriptor *descriptor,
                             const google::protobuf::Reflection *reflection, std::vector<std::string> &vec, int ind,
                             std::string &result);

        bool IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                             const google::protobuf::Descriptor *descriptor,
                             const google::protobuf::Reflection *reflection, std::vector<std::string> &vec, int ind,
                             double &result);

        bool IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                             const google::protobuf::Descriptor *descriptor,
                             const google::protobuf::Reflection *reflection, std::vector<std::string> &vec, int ind,
                             float &result);

        bool IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                             const google::protobuf::Descriptor *descriptor,
                             const google::protobuf::Reflection *reflection, std::vector<std::string> &vec, int ind,
                             bool &result);

        bool IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                             const google::protobuf::Descriptor *descriptor,
                             const google::protobuf::Reflection *reflection, std::vector<std::string> &vec, int ind,
                             int &result);

        bool IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                             const google::protobuf::Descriptor *descriptor,
                             const google::protobuf::Reflection *reflection, std::vector<std::string> &vec, int ind,
                             long long &result);

        bool IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                             const google::protobuf::Descriptor *descriptor,
                             const google::protobuf::Reflection *reflection, std::vector<std::string> &vec, int ind,
                             unsigned int &result);

        bool IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                             const google::protobuf::Descriptor *descriptor,
                             const google::protobuf::Reflection *reflection, std::vector<std::string> &vec, int ind,
                             unsigned long long &result);

        bool ExtractIndex(const std::string &inputString, std::string &outString, int &index);

    private:
        std::string messageName;
        std::string messageContent;
        std::shared_ptr<google::protobuf::Message> msg;
    };

    bool pbpath::ParseBinary() {
        google::protobuf::Message *mess = NULL;
        const google::protobuf::Descriptor *desc = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(
                messageName);
        if (desc != nullptr) {
            const google::protobuf::Message *prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(
                    desc);
            if (prototype) {
                mess = prototype->New();
                mess->ParseFromString(messageContent);
                msg = std::shared_ptr<google::protobuf::Message>(mess);
                return true;
            }
            //return false;
        }
        return false;
    }

    bool pbpath::ExtractIndex(const std::string &inputString, std::string &outString, int &index) {
        if (inputString[inputString.length() - 1] == ']') {
            //auto pos = inputString.find_last_of("[");
            auto pos = inputString.length() - 1;
            while (inputString[pos] != '[') {
                --pos;
            }
            index = std::stoi(inputString.substr(pos + 1, inputString.length()));
            outString = inputString.substr(0, pos);
            return true;
        }
        outString = inputString;
        return false;
    }

    bool pbpath::IterateGetField(const std::shared_ptr<google::protobuf::Message> ptr,
                                 const google::protobuf::Descriptor *descriptor,
                                 const google::protobuf::Reflection *reflection, std::vector<std::string> &fieldSets,
                                 int ind, std::string &result) {
        if (ind >= fieldSets.size()) return false;
        std::string fdName;
        int arrayIndex = -1;
        bool tag = ExtractIndex(fieldSets[ind], fdName, arrayIndex);
        const google::protobuf::FieldDescriptor *fd = descriptor->FindFieldByName(fdName);
        if (!((!fd->is_repeated() && !tag) || (fd->is_repeated() && tag))) return false;
        if (!fd->is_repeated()) {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_STRING) {
                result = reflection->GetString(*ptr, fd);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = reflection->GetMessage(*ptr, fd);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        } else {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_STRING) {
                result = reflection->GetRepeatedString(*ptr, fd, arrayIndex);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = reflection->GetRepeatedMessage(*ptr, fd, arrayIndex);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        }
    }

    bool pbpath::IterateGetField(const std::shared_ptr<google::protobuf::Message> inMess,
                                 const google::protobuf::Descriptor *desc, const google::protobuf::Reflection *refl,
                                 std::vector<std::string> &fieldSets, int ind, double &result) {
        if (ind >= fieldSets.size()) return false;
        std::string fdName;
        int arrayIndex = -1;
        bool tag = ExtractIndex(fieldSets[ind], fdName, arrayIndex);
        const google::protobuf::FieldDescriptor *fd = desc->FindFieldByName(fdName);
        if (!((!fd->is_repeated() && !tag) || (fd->is_repeated() && tag))) return false;
        if (!fd->is_repeated()) {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE) {
                result = refl->GetDouble(*inMess, fd);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetMessage(*inMess, fd);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        } else {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE) {
                result = refl->GetRepeatedDouble(*inMess, fd, arrayIndex);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetRepeatedMessage(*inMess, fd, arrayIndex);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        }
    }

    bool pbpath::IterateGetField(const std::shared_ptr<google::protobuf::Message> inMess,
                                 const google::protobuf::Descriptor *desc, const google::protobuf::Reflection *refl,
                                 std::vector<std::string> &fieldSets, int ind, float &result) {
        if (ind >= fieldSets.size()) return false;
        std::string fdName;
        int arrayIndex = -1;
        bool tag = ExtractIndex(fieldSets[ind], fdName, arrayIndex);
        const google::protobuf::FieldDescriptor *fd = desc->FindFieldByName(fdName);
        if (!((!fd->is_repeated() && !tag) || (fd->is_repeated() && tag))) return false;
        if (!fd->is_repeated()) {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_FLOAT) {
                result = refl->GetFloat(*inMess, fd);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetMessage(*inMess, fd);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        } else {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_FLOAT) {
                result = refl->GetRepeatedFloat(*inMess, fd, arrayIndex);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetRepeatedMessage(*inMess, fd, arrayIndex);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        }
    }

    bool pbpath::IterateGetField(const std::shared_ptr<google::protobuf::Message> inMess,
                                 const google::protobuf::Descriptor *desc, const google::protobuf::Reflection *refl,
                                 std::vector<std::string> &fieldSets, int ind, bool &result) {
        if (ind >= fieldSets.size()) return false;
        std::string fdName;
        int arrayIndex = -1;
        bool tag = ExtractIndex(fieldSets[ind], fdName, arrayIndex);
        const google::protobuf::FieldDescriptor *fd = desc->FindFieldByName(fdName);
        if (!((!fd->is_repeated() && !tag) || (fd->is_repeated() && tag))) return false;
        if (!fd->is_repeated()) {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_BOOL) {
                result = refl->GetBool(*inMess, fd);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetMessage(*inMess, fd);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        } else {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_BOOL) {
                result = refl->GetRepeatedBool(*inMess, fd, arrayIndex);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetRepeatedMessage(*inMess, fd, arrayIndex);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        }
    }

    bool pbpath::IterateGetField(const std::shared_ptr<google::protobuf::Message> inMess,
                                 const google::protobuf::Descriptor *desc, const google::protobuf::Reflection *refl,
                                 std::vector<std::string> &fieldSets, int ind, int &result) {
        if (ind >= fieldSets.size()) return false;
        std::string fdName;
        int arrayIndex = -1;
        bool tag = ExtractIndex(fieldSets[ind], fdName, arrayIndex);
        const google::protobuf::FieldDescriptor *fd = desc->FindFieldByName(fdName);
        if (!((!fd->is_repeated() && !tag) || (fd->is_repeated() && tag))) return false;
        if (!fd->is_repeated()) {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_INT32 ||
                fd->type() == google::protobuf::FieldDescriptor::TYPE_FIXED32) {
                result = refl->GetInt32(*inMess, fd);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetMessage(*inMess, fd);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        } else {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_INT32 ||
                fd->type() == google::protobuf::FieldDescriptor::TYPE_FIXED32) {
                result = refl->GetRepeatedInt32(*inMess, fd, arrayIndex);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetRepeatedMessage(*inMess, fd, arrayIndex);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        }
    }

    bool pbpath::IterateGetField(const std::shared_ptr<google::protobuf::Message> inMess,
                                 const google::protobuf::Descriptor *desc, const google::protobuf::Reflection *refl,
                                 std::vector<std::string> &fieldSets, int ind, long long &result) {
        if (ind >= fieldSets.size()) return false;
        std::string fdName;
        int arrayIndex = -1;
        bool tag = ExtractIndex(fieldSets[ind], fdName, arrayIndex);
        const google::protobuf::FieldDescriptor *fd = desc->FindFieldByName(fdName);
        if (!((!fd->is_repeated() && !tag) || (fd->is_repeated() && tag))) return false;
        if (!fd->is_repeated()) {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_INT64 ||
                fd->type() == google::protobuf::FieldDescriptor::TYPE_FIXED64) {
                result = refl->GetInt64(*inMess, fd);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetMessage(*inMess, fd);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        } else {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_INT64 ||
                fd->type() == google::protobuf::FieldDescriptor::TYPE_FIXED64) {
                result = refl->GetRepeatedInt64(*inMess, fd, arrayIndex);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetRepeatedMessage(*inMess, fd, arrayIndex);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        }
    }

    bool pbpath::IterateGetField(const std::shared_ptr<google::protobuf::Message> inMess,
                                 const google::protobuf::Descriptor *desc, const google::protobuf::Reflection *refl,
                                 std::vector<std::string> &fieldSets, int ind, unsigned int &result) {
        if (ind >= fieldSets.size()) return false;
        std::string fdName;
        int arrayIndex = -1;
        bool tag = ExtractIndex(fieldSets[ind], fdName, arrayIndex);
        const google::protobuf::FieldDescriptor *fd = desc->FindFieldByName(fdName);
        if (!((!fd->is_repeated() && !tag) || (fd->is_repeated() && tag))) return false;
        if (!fd->is_repeated()) {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_UINT32) {
                result = refl->GetUInt32(*inMess, fd);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetMessage(*inMess, fd);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        } else {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_UINT32) {
                result = refl->GetRepeatedInt64(*inMess, fd, arrayIndex);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetRepeatedMessage(*inMess, fd, arrayIndex);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        }
    }

    bool pbpath::IterateGetField(const std::shared_ptr<google::protobuf::Message> inMess,
                                 const google::protobuf::Descriptor *desc, const google::protobuf::Reflection *refl,
                                 std::vector<std::string> &fieldSets, int ind, unsigned long long &result) {
        if (ind >= fieldSets.size()) return false;
        std::string fdName;
        int arrayIndex = -1;
        bool tag = ExtractIndex(fieldSets[ind], fdName, arrayIndex);
        const google::protobuf::FieldDescriptor *fd = desc->FindFieldByName(fdName);
        if (!((!fd->is_repeated() && !tag) || (fd->is_repeated() && tag))) return false;
        if (!fd->is_repeated()) {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_UINT64) {
                result = refl->GetUInt64(*inMess, fd);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetMessage(*inMess, fd);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        } else {
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_UINT64) {
                result = refl->GetRepeatedUInt64(*inMess, fd, arrayIndex);
                return true;
            }
            if (fd->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
                const google::protobuf::Message &mfield = refl->GetRepeatedMessage(*inMess, fd, arrayIndex);
                google::protobuf::Message *mCopyMess = mfield.New();
                mCopyMess->CopyFrom(mfield);
                std::shared_ptr<google::protobuf::Message> messPtr(mCopyMess);
                const google::protobuf::Descriptor *pDesc = messPtr->GetDescriptor();
                const google::protobuf::Reflection *pRefl = messPtr->GetReflection();
                if (IterateGetField(messPtr, pDesc, pRefl, fieldSets, ind + 1, result)) return true;
            }
            return false;
        }
    }

    int pbpath::GetFieldValue(std::string &fieldName, std::string &strValue) {
        //miss char->string;
        StringUtils strUtils;
        std::vector<std::string> fieldSets;
        strUtils.split(fieldName, '.', fieldSets);
        const google::protobuf::Descriptor *desc = msg->GetDescriptor();
        assert(NULL != desc);
        const google::protobuf::Reflection *refl = msg->GetReflection();
        assert(NULL != refl);
        if (IterateGetField(msg, desc, refl, fieldSets, 0, strValue)) return 0;
        return -1;
    }

    int pbpath::GetFieldValue(std::string &fieldName, double &Value) {

        StringUtils strUtils;
        std::vector<std::string> fieldSets;
        strUtils.split(fieldName, '.', fieldSets);

        const google::protobuf::Descriptor *desc = msg->GetDescriptor();
        assert(NULL != desc);
        const google::protobuf::Reflection *refl = msg->GetReflection();
        assert(NULL != refl);
        if (IterateGetField(msg, desc, refl, fieldSets, 0, Value)) return 0;
        return -1;
    }

    int pbpath::GetFieldValue(std::string &fieldName, float &Value) {

        StringUtils strUtils;
        std::vector<std::string> fieldSets;
        strUtils.split(fieldName, '.', fieldSets);
        std::cout << "this float" << std::endl;
        const google::protobuf::Descriptor *desc = msg->GetDescriptor();
        assert(NULL != desc);
        const google::protobuf::Reflection *refl = msg->GetReflection();
        assert(NULL != refl);
        if (IterateGetField(msg, desc, refl, fieldSets, 0, Value)) return 0;
        return -1;
    }

    int pbpath::GetFieldValue(std::string &fieldName, bool &Value) {

        StringUtils strUtils;
        std::vector<std::string> fieldSets;
        strUtils.split(fieldName, '.', fieldSets);

        const google::protobuf::Descriptor *desc = msg->GetDescriptor();
        assert(NULL != desc);
        const google::protobuf::Reflection *refl = msg->GetReflection();
        assert(NULL != refl);
        if (IterateGetField(msg, desc, refl, fieldSets, 0, Value)) return 0;
        return -1;
    }

    int pbpath::GetFieldValue(std::string &fieldName, int &Value) {

        StringUtils strUtils;
        std::vector<std::string> fieldSets;
        strUtils.split(fieldName, '.', fieldSets);

        const google::protobuf::Descriptor *desc = msg->GetDescriptor();
        assert(NULL != desc);
        const google::protobuf::Reflection *refl = msg->GetReflection();
        assert(NULL != refl);
        if (IterateGetField(msg, desc, refl, fieldSets, 0, Value)) return 0;
        return -1;
    }

    int pbpath::GetFieldValue(std::string &fieldName, long long &Value) {

        StringUtils strUtils;
        std::vector<std::string> fieldSets;
        strUtils.split(fieldName, '.', fieldSets);

        const google::protobuf::Descriptor *desc = msg->GetDescriptor();
        assert(NULL != desc);
        const google::protobuf::Reflection *refl = msg->GetReflection();
        assert(NULL != refl);
        if (IterateGetField(msg, desc, refl, fieldSets, 0, Value)) return 0;
        return -1;
    }

    int pbpath::GetFieldValue(std::string &fieldName, unsigned int &Value) {

        StringUtils strUtils;
        std::vector<std::string> fieldSets;
        strUtils.split(fieldName, '.', fieldSets);

        const google::protobuf::Descriptor *desc = msg->GetDescriptor();
        assert(NULL != desc);
        const google::protobuf::Reflection *refl = msg->GetReflection();
        assert(NULL != refl);
        if (IterateGetField(msg, desc, refl, fieldSets, 0, Value)) return 0;
        return -1;
    }

    int pbpath::GetFieldValue(std::string &fieldName, unsigned long long &Value) {

        StringUtils strUtils;
        std::vector<std::string> fieldSets;
        strUtils.split(fieldName, '.', fieldSets);

        const google::protobuf::Descriptor *desc = msg->GetDescriptor();
        assert(NULL != desc);
        const google::protobuf::Reflection *refl = msg->GetReflection();
        assert(NULL != refl);
        if (IterateGetField(msg, desc, refl, fieldSets, 0, Value)) return 0;
        return -1;
    }

} // end namespace cfets_protobuf
