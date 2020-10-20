/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the 
 * EventListener class.
 */
#ifndef __EVENTLISTENER_HPP__
#define __EVENTLISTENER_HPP__

#include <typeindex>
#include <set>
#include <string>
#include <cstring> // size_t

#include "VelEng/Event.hpp"

namespace ge{
    /**
     * @brief Abstract base class for objects that use the 
     * [event system](@ref EventSystem).
     */
    class EventListener{
    public:
        /**
         * @brief Names of the channels the listener should subscribe to.
         * @see EventSystem::subscribe EventSystem::unsubscribe
         */
        std::set<std::string> m_channels;
        /**
         * @brief Receive an event and process it.
         * @param event Pointer to the event.
         * @param type Type information about the event.
         * @param channel ID of the channel from the @ref EventSystem 
         * that delivered the event.
         */
        virtual void notify(const Event* event, std::type_index type, size_t channel) = 0;
    };
}

#endif
