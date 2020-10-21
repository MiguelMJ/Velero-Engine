#include "VelEng/Scene.hpp"

#include "VelEng/Serialize/Parser.hpp"
#include "VelEng/AssetSystem.hpp"
#include "VelEng/Context.hpp"

namespace ven{
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
    bool Scene::loadFromFile(const std::string& filename){
        rapidjson::Document json;
        std::ifstream fin(filename);
        rapidjson::IStreamWrapper iws(fin);
        rapidjson::ParseResult ok = json.ParseStream(iws);
        fin.close();
        CHECK_F(!!ok,
                "JSON parse error: {} ({})",
                rapidjson::GetParseError_En(ok.Code()),
                ok.Offset());
        for(auto& prototype: json.GetArray()){
            Prototype *prot;
            float x = DOMget<float>(prototype,"y",0);
            float y = DOMget<float>(prototype,"x",0);
            bool expl = DOMget<bool>(prototype,"explicit",false);
            std::string name = DOMget<std::string>(prototype, "name","");
            if(expl){
                prot = new Prototype;
                prot->loadFromJSON(prototype);
                addEntity(prot, false, name)->setPosition(sf::Vector2f(x,y));
                delete prot;
            }else{
                std::string path = DOMget<std::string>(prototype,"path");
                prot = M_AS::getPrototype(path);
                addEntity(prot, false, name)->setPosition(sf::Vector2f(x,y));
            }
        }
        return true;
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
