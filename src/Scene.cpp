#include "Scene.hpp"

namespace ge{
    Entity* Scene::addEntity(bool active, const std::string& name){
        unsigned long id = nextID++;
        auto e = new Entity(id, name, active);
        m_entities.insert(identitypair(nextID++, std::unique_ptr<Entity>(e)));
        if(!name.empty()){
            m_entityIndex[name]=id;
        }
        return e;
    }
    void Scene::removeEntity(unsigned long id){
        auto e = m_entities.at(id).get();
        m_entityIndex.erase(m_entityIndex.find(e->getName()));
        m_entities.erase(m_entities.find(id));
    }
}
