#ifndef __ENTITYCOMPONENT_HPP__
#define __ENTITYCOMPONENT_HPP__

#include "SFML/Graphics.hpp"

#include "EventListener.hpp"
#include "EventSystem.hpp"

#include <map>

namespace ge{
    typedef std::pair<std::type_index, std::type_index> typepair;
    class Entity; // fw declaration
    class Component{
    private:
        bool m_active;
    protected:
        friend class Entity;
        Entity* m_ptrEntity=nullptr;
        std::set<std::type_index> m_eventsHandled;
        std::set<std::string> m_channels;
    public:
        void setActive(bool);
        // onCreate es necesaria porque el constructor que se usará
        // en Prototype será siempre el de copia
        virtual void onAdd();
        virtual void onRemove();
        virtual void onActivate();
        virtual void onDeactivate();
        virtual void handle(const Event*, std::type_index, size_t);
    };
    
    typedef std::pair < std::type_index , std::unique_ptr<Component> > typecomppair;
    class Entity : public sf::Transformable, public EventListener{
    private:
        unsigned long m_id;
        std::string m_name;
        bool m_active;
        std::multimap<std::type_index, std::unique_ptr<Component> > m_components;
        std::multimap<std::type_index, std::type_index> m_eventHandlers;
        std::map<size_t, int> m_subscribeCount;
    public:
        Entity(unsigned long id=0, std::string name="", bool active=true);
        void notify(const Event*, std::type_index, size_t channel) override;
        template <class T>
        void addComponent(const T& c, bool active=true){
            std::type_index ct(typeid(T));
            Component* cptr = new T(c);
            cptr->m_ptrEntity = this;
            for(auto& ch : cptr->m_channels){
                size_t chid = EventSystem::getChannelId(ch);
                m_subscribeCount[chid]++;
                this->EventListener::m_channels.insert(ch);
            }
            for(auto& et : cptr->m_eventsHandled){
                m_eventHandlers.insert(typepair(et,ct));
            }
            cptr->onAdd();
            cptr->setActive(active);
            m_components.insert(typecomppair(ct,std::unique_ptr<Component>(cptr)));
        }
        void removeComponents(std::type_index);
        void setActive(bool active);
        bool isActive();
        unsigned long getId();
        std::string getName();
        ~Entity();
    };
}

#endif
