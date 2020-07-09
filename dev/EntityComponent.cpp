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
    void Component::onActivate(){}
    void Component::onDeactivate(){}
    void Component::handle(const Event*,std::type_index,size_t){}
    void Entity::notify(const Event* e, std::type_index t, size_t ch){
        auto iteh = m_eventHandlers.find(t);
        while(iteh != m_eventHandlers.end() && iteh->first == t){
            auto itc = m_components.find(iteh->second);
            while(itc != m_components.end() && itc->first == iteh->second){
                itc->second->handle(e,t,ch);
                itc++;
            }
            iteh++;
        }
    }
    void Entity::removeComponents(std::type_index t){
        auto itc = m_components.find(t);
        if(itc == m_components.end()){
            return;
        }
        auto& channels = itc->second->m_channels;
        for(auto& ch : channels){
            size_t chid = EventSystem::getChannelId(ch);
            this->m_subscribeCount[chid]--;
            if(this->m_subscribeCount[chid] <= 0){
               this->EventListener::m_channels.erase(ch);
            }
        }
        auto iteh = m_eventHandlers.begin();
        while(iteh != m_eventHandlers.end()){
            if(iteh->second == t){
                iteh = m_eventHandlers.erase(iteh);
            }else{
                iteh++;
            }
        }
        m_components.erase(t);
    }
    unsigned long Entity::getId(){
        return m_id;
    }
    std::string Entity::getName(){
        return m_name;
    }
}
