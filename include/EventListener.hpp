#ifndef __EVENTLISTENER_HPP__
#define __EVENTLISTENER_HPP__

#include <typeindex>
#include <set>
#include <string>
#include <cstring> // size_t

#include "Event.hpp"

namespace ge{
    /* 
     * 
     */
    class EventListener{
    public:
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
