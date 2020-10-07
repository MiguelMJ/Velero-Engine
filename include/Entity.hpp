/** 
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the Entity class
 * and a convenient typedef.
 */

#ifndef __ENTITYCOMPONENT_HPP__
#define __ENTITYCOMPONENT_HPP__

#include "SFML/Graphics.hpp"

#include "EventListener.hpp"
#include "EventSystem.hpp"

#include <map>
#include "Component.hpp"

namespace ge{
    typedef std::pair < std::type_index , std::unique_ptr<Component> > typecomppair;
    
    /**
     * @brief Game objects, defined by the [components](@ref Component) that it 
     * contains.
     * @details Entities represent objects in the game world. Thus, they 
     * are defined by a transform (it is a child class of sf::Transform),
     * a unique identifier and an optional unique name. 
     * 
     * They are the minimum unit of referable information in the game 
     * world. This means that the components that form the entities are 
     * not accesible from the outside. The interactions between the
     * entities should be designed around the exchange of @ref Event 
     * objects, be it via the @ref EventSystem or directly by the
     * [notify](@ref notify(const T&)) function.
     */
    class Entity final: public sf::Transformable, public EventListener{
    private:
        unsigned long m_id;
        std::string m_name;
        bool m_active;
        std::multimap<std::type_index, std::unique_ptr<Component> > m_components;
        std::multimap<std::type_index, std::type_index> m_eventHandlers; // <Event type, Component type>
        std::map<size_t, int> m_subscribeCount; // <channel id, count>
        friend class Prototype;
    public:
        /**
         * @brief Constructor.
         * @param id Asigned identifier.
         * @param name Asigned name.
         * @param active Initial value of the _active_ flag.
         */
        Entity(unsigned long id=0, std::string name="", bool active=true);
        
        /**
         * @brief @copybrief EventListener::notify
         * @details The entity looks up which of its components are able 
         * to handle [events](@ref Event) of that @p type, and passes the event to
         * them.
         * @param event Pointer to the event.
         * @param type Type information about the event.
         * @param channel ID of the channel from the @ref EventSystem 
         * that delivered the event. If the event doesn't come from that system,
         * then its value is -1.
         */
        void notify(const Event* event, std::type_index type, size_t channel) override;
        
        /**
         * @brief @copybrief notify
         * @details This method overloads its more 
         * [general version](@ref notify) to
         * be used outside the EventSystem.
         * @param event Object inheriting from @ref Event.  
         */
        template <class T>
        void notify(const T& event){
            notify(&event, typeid(T), -1);
        }
        
        /**
         * @brief Add a component and set its _active_ flag.
         * @details A copy of the @p component is added. The active flag 
         * of the component is set as an and operation between the 
         * parameter @p active and the _active_ flag of the entity. The 
         * Entity will register to the channels and 
         * events associated to it.
         * @param component Pointer to object inheriting from 
         * @ref Component.
         * @param active Value to set the _active_ flag of 
         * the @p component.
         * @see Component::onAdd Component::setActive Component::copy
         */
        void addComponentFromPtr(const Component* component, bool active=true);
        
        /**
         * @brief @copybrief addComponentFromPtr
         * @details This is a copy of @ref addComponentFromPtr that prevents 
         * the use of pointers to the user.
         * @param component Object inheriting from @ref Component
         * @param active Value to set the _active_ flag of 
         * the @p component.
         * @see addComponentFromPtr
         * 
         */
        template <class T>
        void addComponent(const T& component, bool active=true){
            addComponentFromPtr(&component, active);
        }
        
        /**
         * @brief Remove all components of a given @p type.
         * @details If the removed component is the last one associated 
         * to a channel, then the entity will unsubscribe from it.
         * @param type Type of the [components](@ref Component) to be removed.
         * @see Component::onRemove
         */
        void removeComponents(std::type_index type);
        
        /**
         * @brief Set the _active_ flag of the entity.
         * @details If the new value of the _active_ flag is the same as 
         * the old, it has no effect. Otherwise, the _active_ flag is set 
         * the same for all the components of the entity.
         * @param active Value to set the _active_ flag.
         * @see Component::setActive
         */
        void setActive(bool active);
        
        /**
         * @brief Get the _active_ flag of the entity.
         * @returns The _active_ flag.
         * @see isActive
         */
        bool isActive();
        
        /**
         * @brief Get the entity identifier.
         * @returns Entity ID.
         */
        unsigned long getId() const;
        
        /**
         * @brief Get the entity name.
         * @returns Entity name.
         */
        std::string getName() const;
        
        /**
         * @brief Destructor
         * @details Removes all the components.
         * @see Component::onRemove
         */
        ~Entity();
        
        /**
         * @brief Flag to remove the components of the entities in
         * the constructor.
         * @details The app might crash at the end, when the entities
         * remove components from systems that might be already 
         * destroyed (I'm not 100% sure if that is the problem, but
         * this makes the workaround). To prevent this crash, this
         * flag must be set to false somewhere at the end of the
         * application.
         */
        static bool s_removeComponents;
    };
}

#endif
