/** @file EntityComponent.hpp
 *  @author Miguel Mejía Jiménez
 */

#ifndef __ENTITYCOMPONENT_HPP__
#define __ENTITYCOMPONENT_HPP__

#include "SFML/Graphics.hpp"

#include "EventListener.hpp"
#include "EventSystem.hpp"

#include <map>

namespace ge{
    typedef std::pair<std::type_index, std::type_index> typepair;
    class Entity; // fw declaration
    /** 
     * @brief Base class for entity components 
     * @details Every component class must inherit from this class in 
     *          order to be used within an entity. Components are not      
     *          moved into entities, but copied via copy constructor, so
     *          it is necesary in order to function properly. In most
     *          cases, the default copy constructor is enough. 
     */
    class Component{
    private:
        bool m_active;
    protected:
        friend class Entity;
        /** 
         * @member Pointer to the entity that contains this component.
         */
        Entity* m_ptrEntity;
        /** 
         * @member Types of the events that the entity should pass to this
         *         component.
         */
        std::set<std::type_index> m_eventsHandled;
        /**
         * @member Channels of ge::EventSystem to which the entity should
         *         listen while this component is active.
         */
        std::set<std::string> m_channels;
    public:
        /**
         * @brief Set the active flag of the component
         * @details When a component is active, 
         * @param active Value to set the active flag
         */
        void setActive(bool active);
        
        virtual void onAdd();
        virtual void onRemove();
        virtual void onActivate();
        virtual void onDeactivate();
        virtual void handle(const Event*, std::type_index, size_t);
    };
    
    typedef std::pair < std::type_index , std::unique_ptr<Component> > typecomppair;
    class Entity final: public sf::Transformable, public EventListener{
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
