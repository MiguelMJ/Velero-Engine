#include "Scene.hpp"

namespace ge{
    Entity* Scene::addEntity(Prototype* prototype, bool active, const std::string& name){
        auto e = prototype->generate(m_nextID, name, active && m_active);
        m_entities.insert(identitypair(m_nextID, std::unique_ptr<Entity>(e)));
        if(!name.empty()){
            m_entityIndex[name] = m_nextID;
        }
        m_nextID++;
        return e;
    }
    Entity* Scene::addEntity(bool active, const std::string& name){
        auto e = new Entity(m_nextID, name, active && m_active);
        m_entities.insert(identitypair(m_nextID, std::unique_ptr<Entity>(e)));
        if(!name.empty()){
            m_entityIndex[name] = m_nextID;
        }
        m_nextID++;
        return e;
    }
    void Scene::removeEntity(unsigned long id){
        auto it = m_entities.find(id);
        if(it == m_entities.end()){
            LOG_F(WARNING, "Can't remove entity {} that doesn't exist", id);
            return;
        }
        auto e = it->second.get();
        m_entityIndex.erase(m_entityIndex.find(e->getName()));
        m_entities.erase(m_entities.find(id));
    }
    bool Scene::loadFromFile(const std::string& file){
        std::ifstream fin(file);
        if(!fin.good()){
            fin.close();
            return false;
        }
        bool ok = true;
        fin >> std::ws;
        while(ok && !fin.eof()){
            char ch;
            bool gen = false;
            bool del = false;
            Prototype *prot;
            std::string ops;
            fin >> ch;
            if(ch == '{'){
                LOG_SCOPE_F(INFO,"Loading anonymous prototype");
                del = true;
                prot = new Prototype;
                std::string chunk;
                getline(fin, chunk, '}');
                std::stringstream ss(chunk);
                if(prot->loadFromStream(ss)){
                    getline(fin, ops);
                    gen = true;
                }else{
                    LOG_F(ERROR, "Error loading anonymous prototype");
                    ok = false;
                }
            }else if(ch == 'p'){
                getline(fin, ops);
                gen = true;
            }else if(ch == '#'){
                fin.ignore(9999,'\n');
            }else{
                
            }
            if(gen){
                auto opsmap = parseMap(ops);
                std::string src, name;
                sf::Vector2f pos;
                for(auto& kv: opsmap){
                    if(kv.first == "name"){
                        name = kv.second;
                    }else if(kv.first == "pos"){
                        pos = parseVector2<float>(kv.second);
                    }else if(kv.first == "src"){
                        if(del){
                            LOG_F(WARNING, "Ignoring src for anonymous prototype");
                        }else{
                            src = kv.second;
                        }
                    }else{
                        LOG_F(WARNING, "Unrecognized key for Entity: {}", kv.first);
                    }
                }
                if(!del){
                    AssetSystem::load(src);
                    prot = AssetSystem::getPrototype(src);   
                }
                addEntity(prot, false, name)->setPosition(pos);
            }
            if(del){
                delete prot;
            }
            fin >> std::ws;
        }
        return ok;
    }
    void Scene::setActive(bool active){
        if(active != m_active){
            for(auto& iden: m_entities){
                iden.second->setActive(active);
            }
            m_active = active;
        }
    }
}
