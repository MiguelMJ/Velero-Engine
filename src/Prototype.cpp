#include "VelEng/Prototype.hpp"
#include "VelEng/Serialize/Parser.hpp"
#include "rapidjson/rapidjson.h"
#include <fstream>

namespace ven{
    
    Entity* Prototype::generate(unsigned long id, std::string name, bool active){
        Entity* e;
        if (name.empty()) name = fmt::format("{}{:0>3}",m_name,m_generated);
        LOG_SCOPE_F(INFO, "Generating %s (%i) %d", name.c_str(), m_generated, active);
        if(m_base != nullptr){
            e = m_base->generate(active);
            e -> m_name = name;
            e -> m_id = id;
        }else{
            e = new Entity(id,name,active);
        }
        for(auto& c: m_components){
            e->addComponentFromPtr(c.get(), active);
            LOG_F(INFO,"Component - {}",c->to_string());
        }
        m_generated++;
        return e;
    }
    bool Prototype::loadFromJSON(const JSON& json){
        LOG_SCOPE_F(INFO,"Loading prototype");
        m_name = DOMget<std::string>(json, "name");
        LOG_F(INFO,"Name: {}", m_name);
        m_nameOfBase = DOMget<std::string>(json, "base","");
        auto components = json["components"].GetArray();
        for(auto& component: components){
            m_components.emplace_back(ComponentParser::parse(component));
        }
        return true;
    }
    bool Prototype::loadFromString(const std::string& str){
        rapidjson::Document json;
        rapidjson::ParseResult ok = json.Parse(str.c_str());
        CHECK_F(!!ok,
                "JSON parse error: {} ({})",
                rapidjson::GetParseError_En(ok.Code()),
                ok.Offset()
               );
        return loadFromJSON(json);
    }
    bool Prototype::loadFromFile(const std::string& file){
        rapidjson::Document json;
        std::ifstream fin(file);
        rapidjson::IStreamWrapper fw(fin);
        rapidjson::ParseResult ok = json.ParseStream(fw);
        fin.close();
        CHECK_F(!!ok,
                "JSON parse error: {} ({})",
                rapidjson::GetParseError_En(ok.Code()),
                ok.Offset()
               );
        return loadFromJSON(json);
    }
    Prototype::~Prototype(){
        m_components.clear();
    }
}
