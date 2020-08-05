#include "Prototype.hpp"

namespace ge{
    Entity* Prototype::generate(unsigned long id, std::string name, bool active){
        Entity* e;
        if (name.empty()) name = fmt::format("{}{:0>3}",m_name,m_generated);
        LOG_SCOPE_F(INFO, "%s generating (%i)", name.c_str(), m_generated);
        if(m_base != nullptr){
            e = m_base->generate(active);
            e -> m_name = name;
            e -> m_id = id;
        }else{
            e = new Entity(id,name,active);
        }
        for(auto& c: m_components){
            e->addComponentFromPtr(c.get(), active);
            LOG_F(INFO,"{}",c->to_string());
        }
        m_generated++;
        return e;
    }
    bool Prototype::loadFromStream(std::istream& in){
        bool ok = true;
        std::string firstline;
        do{
            getline(in, firstline);
        }while(firstline.empty() || firstline[0] == '#');
        std::stringstream ss(firstline);
        getline(ss,m_name,':');
        getline(ss,m_nameOfBase);
        trim(m_name);
        trim(m_nameOfBase);
        in >> std::ws;
        while(ok && !in.eof()){
            if(in.peek() == '#'){
                in.ignore(9999,'\n');
                in >> std::ws;
                continue;
            }
            Component* c = ComponentParser::parse(in);
            ok = c != nullptr;
            if(ok){
                m_components.emplace_back(c);
            }
            in >> std::ws;
        }
        LOG_IF_F(INFO, !m_nameOfBase.empty(),"Base: {}", m_nameOfBase);
        LOG_F(INFO, "Components: {}", m_components.size());
        return ok;
    }
    bool Prototype::loadFromFile(const std::string& file){
        std::ifstream fin(file);
        if(!fin.good()){
            fin.close();
            return false;
        }
        bool ok = loadFromStream(fin);
        fin.close();
        return ok;
    }
    Prototype::~Prototype(){
        m_components.clear();
    }
}
