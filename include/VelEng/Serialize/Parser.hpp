#ifndef __VELERO_PARSER_HPP__
#define __VELERO_PARSER_HPP__

#include <fstream>

#include "SFML/System/Vector2.hpp"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/error/en.h"

#include "VelEng/log.hpp"

#include <vector>
#include <string>

namespace ven{
    typedef rapidjson::Value JSON;
    template<typename T>
    T castValue(const JSON& memb);
    template<>
    inline int castValue<int> (const JSON& memb){
        return memb.GetInt();
    }
    template<>
    inline bool castValue<bool> (const JSON& memb){
        return memb.GetBool();
    }
    template<>
    inline float castValue<float> (const JSON& memb){
        return memb.GetFloat();
    }
    template<>
    inline std::string castValue<std::string> (const JSON& memb){
        return memb.GetString();
    }
    template<>
    inline std::vector<std::string> castValue<std::vector<std::string>> (const JSON& memb){
        auto arr = memb.GetArray();
        std::vector<std::string> ret;
        for(auto& el : arr){
            ret.push_back(castValue<std::string>(el));
        }
        return ret;
    }
    template<>
    inline std::vector<int> castValue<std::vector<int>> (const JSON& memb){
        auto arr = memb.GetArray();
        std::vector<int> ret;
        for(auto& el : arr){
            ret.push_back(castValue<int>(el));
        }
        return ret;
    }
    template<>
    inline std::vector<float> castValue<std::vector<float>> (const JSON& memb){
        auto arr = memb.GetArray();
        std::vector<float> ret;
        for(auto& el : arr){
            ret.push_back(castValue<float>(el));
        }
        return ret;
    }
    
    template <typename T>
    T DOMget(const JSON& json, const char* id, const T& alt){
        auto it = json.FindMember(id);
        if(it == json.MemberEnd()){
            return alt;
        }else{
            return castValue<T>(it->value);
        }
    }
    template <typename T>
    T DOMget(const JSON& json, const char* id){
        auto it = json.FindMember(id);
        if(it == json.MemberEnd()){
            ABORT_F("Member {} not found while parsing json",id);
        }else{
            return castValue<T>(it->value);
        }
    }
    template <class T>
    T parseDOM(const JSON&){
        ABORT_F("Unimplemented parse call");
    }
    template <class T>
    T parse(const std::string& str){
        rapidjson::Document d;
        rapidjson::ParseResult ok =  d.Parse(str.c_str());
        CHECK_F(!!ok, // convert to bool
                "JSON parse error: {} ({})",
                rapidjson::GetParseError_En(ok.Code()),
                ok.Offset());
        return parseDOM<T>(d);
    }
    template <class T>
    T parsef(const std::string& filename){
        rapidjson::Document d;
        std::ifstream fin(filename.c_str());
        rapidjson::IStreamWrapper isw(fin);
        rapidjson::ParseResult ok =  d.ParseStream(isw);
        fin.close();
        CHECK_F(!!ok, // convert to bool
                "JSON parse error in file {}: {} ({})",
                filename,
                rapidjson::GetParseError_En(ok.Code()),
                ok.Offset());
        return parseDOM<T>(d);
    }
}

#endif
