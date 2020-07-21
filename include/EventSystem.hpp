#ifndef __EVENTSYSTEM_HPP__
#define __EVENTSYSTEM_HPP__

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <queue>
#include <utility>

#include "geutil.hpp"
#include "Event.hpp"
#include "EventListener.hpp"

namespace ge{
    // The  only reason I dont use unique_ptr is because 
    // it makes every container uncopiable, and the channels
    // can't be created that way
    typedef const Event* constevptr;
    typedef std::pair<std::type_index, constevptr> channelentry;
    typedef std::queue<channelentry>  channel;
    
    /**
     * @brief System to publish and listen to [events](@ref Event).
     */
    namespace EventSystem{
        namespace _private_{
            extern std::vector<channel> g_channels;
        }
        void registerListener(EventListener*);
        void unregisterListener(EventListener*);
        void subscribe(size_t, EventListener*);
        void unsubscribe(size_t, EventListener*);
        size_t getChannelId (const std::string&);
        template <class T>
        void publish(size_t ch, const T& event){
            std::type_index ind(typeid(T));
            channelentry ind_ev(ind, &event);
            _private_::g_channels[ch].push(ind_ev);
        }
        bool freeChannel(const std::string&);
        void dispatch();
        void dispatch(size_t);
    }
}

#endif
