#include "VelEng/ComponentParser.hpp"

namespace ven{
    namespace ComponentParser{
        std::map<std::string, cparsefunc> g_parserIndex;
        Component* parse(const JSON& json){
            std::string type = DOMget<std::string>(json, "type");
            auto it = g_parserIndex.find(type);
            CHECK_F(it != g_parserIndex.end(),
                    "Unrecognized component: {}",
                    type);
            return it->second(json["attributes"]);
        }
        
        void registerComponent(const std::string& name, cparsefunc func){
            if(g_parserIndex.find(name) != g_parserIndex.end()){
                LOG_F(ERROR, "Component {} already registered",name);
                return;
            }else{
                LOG_F(INFO, "Component {} registered",name);
            }
            g_parserIndex[name] = func;
        }
    }
}
