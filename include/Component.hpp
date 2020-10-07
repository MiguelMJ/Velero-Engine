/** 
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the Component class
 * and a convenient typedef.
 */

#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "SFML/Graphics.hpp"

#include "Event.hpp"
#include <typeindex>
#include <set>

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
        
        /**
         * @brief Makes a string containing the information of 
         * the component.
         * @details It would be recommendable to make a string that
         * the parser of the component would be able to read.
         * @returns A string with the component information.
         */
        virtual std::string to_string() const = 0;
        /**
         * @brief Get a pointer to the entity containing the component.
         * @returns A pointer to the entity, or null.
         */
        Entity* getEntityPtr() const;
        virtual ~Component();
    };
}

#endif
