#ifndef __EVENTLISTENER_HPP__
#define __EVENTLISTENER_HPP__

#include <typeindex>
#include <set>
#include <string>
#include <cstring> // size_t

#include "Event.hpp"

namespace ge{
    class EventListener{
    public:
        std::set<std::string> m_channels;
        virtual void notify(const Event*, std::type_index, size_t channel) = 0;
    };
}

#endif
