#include "Scene.hpp"

#include "AssetSystem.hpp"

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
        auto iti = m_entityIndex.find(e->getName());
        if(iti != m_entityIndex.end()){
            m_entityIndex.erase(iti);
        }
        m_entities.erase(it);
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
            if(ch == '>'){
                LOG_SCOPE_F(INFO,"Loading anonymous prototype");
                del = true;
                prot = new Prototype;
                std::string chunk;
                getline(fin, chunk, '<');
                std::stringstream ss(chunk);
                if(prot->loadFromStream(ss)){
                    getline(fin, ops);
                    if(!prot->m_nameOfBase.empty()){
                        AssetSystem::load(prot->m_nameOfBase);
                        prot->m_base = AssetSystem::getPrototype(prot->m_nameOfBase);
                    }
                    gen = true;
                    DLOG_F(INFO, "Should generate from anon prot");
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
                LOG_F(WARNING, "Expected '>', 'p' or '#': ignoring until \\n");
                fin.ignore(9999,'\n');
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
                DLOG_F(INFO, "Generating entity");
                addEntity(prot, false, name)->setPosition(pos);
            }
            if(del){
                delete prot;
            }
            fin >> std::ws;
        }
        LOG_F(INFO, "{}({}) entities loaded to scene", entityCount(), namedEntitiesCount());
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
    iterator Scene::begin(){
        return m_entities.begin();
    }
    const_iterator Scene::begin() const{
        return m_entities.begin();
    }
    iterator Scene::end(){
        return m_entities.end();
    }
    const_iterator Scene::end() const{
        return m_entities.end();
    }
    size_t Scene::entityCount() const{
        return m_entities.size();
    }
    size_t Scene::namedEntitiesCount() const{
        return m_entityIndex.size();
    }
}
