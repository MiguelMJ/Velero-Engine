/** 
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the classes for 
 * entities and components
 * and two convenient typedefs.
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
     * @brief Abstract base class for [entity](@ref Entity) components.
     * @details Every component class must inherit from this one in 
     * order to be used within an entity. Components are not      
     * moved into entities, but copied, so the @ref copy function 
     * is necesary in order to function properly. In most
     * cases, the default copy constructor is enough.
     * 
     * The other five virtual functions of this class do nothing by 
     * default. When overriding them, the difference between addition/
     * removal and activation/deactivation should be clear. The first 
     * ones will be called once in the lifetime of the %Component, so 
     * they should contain the complex operations and construct/clean the 
     * data. The others might be called multiple times, without the need 
     * to destroy or reconstruct the component.
     */
    class Component{
    public:
        /**
         * @brief Set the _active_ flag of the component.
         * @details The _active_ flag determines wether the entity 
         * calls the component to handle events. This function will have
         * no effect if the old value of the flag and the new are the 
         * same.
         * @param active Value to set the _active_ flag.
         * @see onActivate onDeactivate
         */
        void setActive(bool active);
    private:
        friend class Entity;
        bool m_active;
    protected:
        /**  
         * Pointer to the  @ref Entity that contains this component.
         */
        Entity* m_ptrEntity;
        
        /** 
         * Types of the [events](@ref Event) that the entity should pass to this
         * component.
         */
        std::set<std::type_index> m_eventsHandled;
        
        /**
         * Channels of @ref EventSystem to which the entity should
         * listen while this component is active.
         */
        std::set<std::string> m_channels;
        
        /** 
         * @brief Function called when the component is added to an 
         * entity.
         * @details Components are always added via the copy
         * constructor, so the logic related to the entity should
         * be defined in this function (not in the constructor).
         * Another usual operation would be 
         * to include the component in the corresponding system.
         * Also, if the channels or events handled depend on any 
         * information  from the entity, their should be set now 
         * because just after this function, the entity will 
         * register both the channels and the events.
         * @see onRemove m_channels m_eventsHandled
         */
        virtual void onAdd();
        
        /**
         * @brief Function called when the component is removed from 
         * an entity.
         * @details This function should contain the cleanup that 
         * requires information from the entity. Also, if the 
         * component is included in some system, it should be removed 
         * from it now.
         * @see onAdd
         */
        virtual void onRemove();
        
        /**
         * @brief Function called when the component is set as active.
         * @details This function should be used to make effective the 
         * presence of the component in the scene. It is called just 
         * before the component enables event handling.
         * @see setActive onDeactivate
         */
        virtual void onActivate();
        
        /**
         * @brief Function called when the component is set as inactive.
         * @details This function should be used to remove the presence
         * of the component in the scene. it is called just after the 
         * component disables event handling.
         * @see setActive onActivate
         */
        virtual void onDeactivate();
        
        /**
         * @brief Process an event received by the entity.
         * @details The usual structure of this function should be a main
         * switch of the @p type and a static cast at the beginning of 
         * each case, before the logic applied to that event.
         * The types  processed and the channels that deliver the event 
         * will be limited to the ones contained in the 
         * @ref m_eventsHandled and @ref m_channels at the moment that 
         * the Component was the entity.
         * @param event Pointer to the @ref Event received.
         * @param type Type information of the event.
         * @param channel ID of the channel from which the event was
         * received. Its value will be -1 when the notification doesn't 
         * come from @ref EventSystem.
         * @see m_eventsHandled m_channels
         */
        virtual void handle(const Event* event, std::type_index type, size_t channel);
        
    public:
        /**
         * @brief Makes a copy of the component.
         * @details The usual implementation would be a single line:
         * @code{.cpp}
         * Component* MyComponent::copy(){
         *     return new MyComponent(*this);
         * }
         * @endcode
         * @returns A pointer to the copy.
         */
        virtual Component* copy() const = 0;
    };
    
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
        unsigned long getId();
        
        /**
         * @brief Get the entity name.
         * @returns Entity name.
         */
        std::string getName();
        
        /**
         * @brief Destructor
         * @details Removes all the components.
         * @see Component::onRemove
         */
        ~Entity();
    };
}

#endif
