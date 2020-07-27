#include "ComponentParser.hpp"

namespace ge{
    namespace ComponentParser{
        std::map<std::string, cparsefunc> g_parserIndex;
        Component* parse(std::istream& in){
            std::string key;
            in >> std::ws >> key >> std::ws;
            if(key.empty()){
                return nullptr;
            }
            auto it = g_parserIndex.find(key);
            if(it == g_parserIndex.end()){
                LOG_F(ERROR, "Unrecognized component: {}", key);
                return nullptr;
            }
            return it->second(in);
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
