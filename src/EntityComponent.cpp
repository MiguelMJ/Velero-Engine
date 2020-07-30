#include "EntityComponent.hpp"

namespace ge{
    void Component::setActive(bool a){
        if(a && !m_active){
            m_active = true;
            onActivate();
        }else if(!a && m_active){
            m_active = false;
            onDeactivate();
        }
    }
    void Component::onAdd(){}
    void Component::onRemove(){}
    void Component::onActivate(){}
    void Component::onDeactivate(){}
    void Component::handle(const Event*,std::type_index,size_t){}
    Component::~Component(){}
    
    Entity::Entity(unsigned long id, std::string name, bool active)
        : m_id(id)
        , m_name(name)
        , m_active(active)
    {}
    void Entity::notify(const Event* e, std::type_index t, size_t ch){
        auto iteh = m_eventHandlers.find(t);
        while(iteh != m_eventHandlers.end() && iteh->first == t){
            auto itc = m_components.find(iteh->second);
            while(itc != m_components.end() && itc->first == iteh->second){
                if(itc->second->m_active){
                    itc->second->handle(e,t,ch);
                }
                itc++;
            }
            iteh++;
        }
    }
    void Entity::addComponentFromPtr(const Component* component, bool active){
        std::type_index ct(typeid(component));
        Component* cptr = component->copy();
        cptr->m_ptrEntity = this;
        cptr->onAdd();
        for(auto& ch : cptr->m_channels){
            size_t chid = EventSystem::getChannelId(ch);
            m_subscribeCount[chid]++;
            auto& evlich = this->EventListener::m_channels;
            if(evlich.find(ch) == evlich.end()){
                evlich.insert(ch);
                EventSystem::subscribe(chid, this);
            }
        }
        for(auto& et : cptr->m_eventsHandled){
            m_eventHandlers.insert(typepair(et,ct));
        }
        cptr->setActive(active && this->m_active);
        m_components.insert(typecomppair(ct,std::unique_ptr<Component>(cptr)));
    }
    void Entity::removeComponents(std::type_index t){
        auto itcm = m_components.find(t);
        if(itcm == m_components.end()){
            return;
        }
        while(itcm != m_components.end() && itcm->first == t){
            auto& channels = itcm->second->m_channels;
            for(auto& ch : channels){
                size_t chid = EventSystem::getChannelId(ch);
                this->m_subscribeCount[chid]--;
                if(this->m_subscribeCount[chid] <= 0){
                    this->EventListener::m_channels.erase(ch);
                    ge::EventSystem::unsubscribe(chid, this);
                }
            }
            itcm->second->onRemove();
            itcm = m_components.erase(itcm);
        }
        auto iteh = m_eventHandlers.begin();
        while(iteh != m_eventHandlers.end()){
            if(iteh->second == t){
                iteh = m_eventHandlers.erase(iteh);
            }else{
                iteh++;
            }
        }
    }
    void Entity::setActive(bool active){
        for(auto& c: m_components){
            c.second->setActive(active);
        }
        m_active = active;
    }
    bool Entity::isActive(){
        return m_active;
    }
    unsigned long Entity::getId() const{
        return m_id;
    }
    std::string Entity::getName() const{
        return m_name;
    }
    Entity::~Entity(){
        if(s_removeComponents){
            for(auto& it : m_components){
                it.second->onRemove();
            }
        }
    }
    bool Entity::s_removeComponents=true;
}
