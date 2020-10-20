#include "VelEng/ComponentParser.hpp"

namespace ven{
    namespace ComponentParser{
        std::map<std::string, cparsefunc> g_parserIndex;
        Component* parse(std::istream& in){
            std::string key, desc;
            in >> std::ws >> key >> std::ws;
            if(key.back() == '{'){
                key.pop_back();
            }else{
                char st;
                in >> st >> std::ws;
                CHECK_F(st == '{',"Expected {{, got {} for key {}", st, key);
            }
            if(key.empty()){
                LOG_F(WARNING, "Empty component");
                return nullptr;
            }
            auto it = g_parserIndex.find(key);
            if(it == g_parserIndex.end()){
                LOG_F(ERROR, "Unrecognized component: {}", key);
                return nullptr;
            }
            getline(in, desc, '}');
            return it->second(desc);
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
