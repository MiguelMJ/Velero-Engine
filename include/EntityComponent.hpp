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
    
    /** @class Component
     * @brief Base class for @ref Entity components.
     * @details Every component class must inherit from this class in 
     * order to be used within an entity. Components are not      
     * moved into entities, but copied via copy constructor, so
     * it is necesary in order to function properly. In most
     * cases, the default copy constructor is enough.
     */
    class Component{
    private:
        bool m_active; /// @brief Active flag @see setActive
    protected:
        friend class Entity;
        
        /**  
         * Pointer to the  @ref Entity that contains this component.
         */
        Entity* m_ptrEntity;
        
        /** 
         * Types of the @ref Events that the entity should pass to this
         * component.
         */
        std::set<std::type_index> m_eventsHandled;
        
        /**
         * Channels of @ref EventSystem to which the entity should
         * listen while this component is active.
         */
        std::set<std::string> m_channels;
    
    public:
        
        /**
         * @brief Set the _active_ flag of the component.
         * @details The _active_ flag determines wether the @ref Entity 
         * calls the component to handle events.
         * @param active Value to set the _active_ flag.
         * @see onActivate onDeactivate
         */
        void setActive(bool active);
        
        /** 
         * @brief Function called when the component is added to an
         * @ref Entity.
         * @details Components are always added via the copy
         * constructor, so the logic related to the @ref Entity should
         * be defined in this function. Another usual operation would be to 
         * include the component in the corresponding system.
         * Also, if the channels or events handled depend on any information 
         * from the @ref Entity, their should be set now because just after 
         * this function, the @ref Entity will register both the channels
         * and the events.
         * @see onRemove m_channels m_eventsHandled
         */
        virtual void onAdd();
        
        /**
         * @brief Function called when the component is removed from 
         * an @ref Entity.
         * @details This function should contain the cleanup that requires
         * information from the @ref Entity. Also, if the component is
         * included in some system, it should be removed from it now.
         * @see onAdd
         */
        virtual void onRemove();
        
        /**
         * @brief Function called when the component is set as active.
         * @details This function should be used to make effective the 
         * presence of the component in the scene. This function is called
         * just before the component enables event handling.
         * @see setActive onDeactivate
         */
        virtual void onActivate();
        
        /**
         * @brief Function called when the component is set as inactive.
         * @details This function should be used to remove the presence
         * of the component in the scene. This function is called just
         * after the component disables event handling.
         */
        virtual void onDeactivate();
        
        /**
         * @brief Process an @ref Event received by the @ref Entity.
         * @details 
         * @param event Pointer to the @ref Event received.
         * @param type Type information of the event.
         * @param channel ID of the channel from which the event was
         * received.
         * @see m_eventsHandled m_channels
         */
        virtual void handle(const Event* event, std::type_index type, size_t channel);
    };
    
    typedef std::pair < std::type_index , std::unique_ptr<Component> > typecomppair;
    /**
     * @brief Entity class
     */
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
            cptr->onAdd();
            for(auto& ch : cptr->m_channels){
                size_t chid = EventSystem::getChannelId(ch);
                m_subscribeCount[chid]++;
                this->EventListener::m_channels.insert(ch);
            }
            for(auto& et : cptr->m_eventsHandled){
                m_eventHandlers.insert(typepair(et,ct));
            }
            cptr->setActive(active);
            m_components.insert(typecomppair(ct,std::unique_ptr<Component>(cptr)));
        }
        void removeComponents(std::type_index);
        void setActive(bool active);
        bool isActive();
        /**
         * @return Entity ID.
         */
        unsigned long getId();
        std::string getName();
        ~Entity();
    };
}

#endif
