#include "Prototype.hpp"

namespace ge{
    Entity* Prototype::generate(bool active){
        Entity* e;
        std::string name = fmt::format("{}{:0>3}",m_name,m_generated);
        if(m_base != nullptr){
            e = m_base->generate(active);
            e -> m_name = name;
        }else{
            e = new Entity(0,name,active);
        }
        for(auto& c: m_components){
            e->addComponentFromPtr(c.get(), active);
        }
        m_generated++;
        return e;
    }
    bool Prototype::loadFromFile(const std::string& file){
        std::ifstream fin(file);
        if(!fin.good()){
            fin.close();
            return false;
        }
        bool ok = true;
        std::string firstline;
        getline(fin, firstline);
        std::stringstream ss(firstline);
        getline(ss,m_name,':');
        getline(ss,m_nameOfBase);
        trim(m_name);
        trim(m_nameOfBase);
        fin >> std::ws;
        while(ok && !fin.eof()){
            Component* c = ComponentParser::parse(fin);
            ok = c != nullptr;
            if(ok){
                m_components.emplace_back(c);
            }
            fin >> std::ws;
        }
        fin.close();
        return ok;
    }
    Prototype::~Prototype(){
        m_components.clear();
    }
}
