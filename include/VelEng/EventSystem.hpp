/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the namespace
 * with the functions of the Event System.
 */
#ifndef __EVENTSYSTEM_HPP__
#define __EVENTSYSTEM_HPP__

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <queue>
#include <utility>

#include "VelEng/log.hpp"
#include "VelEng/Event.hpp"
#include "VelEng/EventListener.hpp"

namespace ge{
    // The  only reason I dont use unique_ptr is because 
    // it makes every container uncopiable, and the channels
    // can't be created that way
    typedef const Event* constevptr;
    typedef std::pair<std::type_index, constevptr> channelentry;
    typedef std::queue<channelentry>  channel;
    
    /**
     * @brief System to publish and listen to [events](@ref Event).
     * @details The main elements of the %Event System are the _channels_ 
     * and the _subscribers_. Every channel has a name, a unique 
     * identifier and a list of subscribers, objects instantiating @ref 
     * EventListener. 
     * Channels are automatically created when they are needed, so the 
     * user doesn't have to worry about doing it explicitly, but it is 
     * convenient to free them when they are no longer necessary. A 
     * thoughtful design around the usage of this system would make easy 
     * to know which channels to free, for example, during a scene change. 
     */
    namespace EventSystem{
        namespace _private_{
            extern std::vector<channel> g_channels;
        }
        
        /**
         * @brief Add a listener to every channel associated to it.
         * @details The listener is registered to its 
         * [channels](@ref EvetListener::m_channels). If a channel 
         * doesn't exist, it is created.
         * @param listener Pointer to the listener to register.
         * @see unregisterListener subscribe
         */
        void registerListener(EventListener* listener);
        
        /**
         * @brief Remove a listener from every channel associated to it.
         * @details The listener is unregistered from its 
         * [channels](@ref EventListener::m_channels). If it is 
         * subscribed to any other channel, it should be 
         * unsubscribed manually. Note that if a channel doesn't exist,
         * will be created and left empty.
         * @param listener Pointer to the listener to register.
         * @see registerListener unsubscribe
         */
        void unregisterListener(EventListener* listener);
        
        /**
         * @brief Adds a listener to a channel.
         * @details The listener is registered to the channel, but it 
         * wont add it to the 
         * [listener channels](@ref EventListener::m_channels). 
         * This function won't create an inexistent channel, but produce
         * an error if @p channel_id isn't associated to a channel.
         * @param channel_id Numeric identifier of the channel.
         * @param listener Pointer to the listener to subscribe.
         * @see unsubscribe
         */
        void subscribe(size_t channel_id, EventListener* listener);
        
        /**
         * @brief Removes a listener from a channel.
         * @details This function won't create an inexistent channel, but 
         * produce an error if @p channel_id isn't associated to a 
         * channel.
         * @param channel_id Numeric identifier of the channel.
         * @param listener Pointer to the listener to unsubscribe.
         * @see subscribe
         */
        void unsubscribe(size_t channel_id, EventListener* listener);
        
        /**
         * @brief Returns the numeric ID of a channel, given its name.
         * @details If the channel doesn't exist, it will be created.
         * @param name Channel name.
         * @returns Channel numeric identifier.
         */
        size_t getChannelId (const std::string& name);
        
        /**
         * @brief Put an event in a channel.
         * @details The event is put in the queue of the channel, but is 
         * not delievered to the subscribers yet.
         * @param channel_id Channel numeric identifier.
         * @param event Object inheriting from @ref Event.
         * @see getChannelId dispatch
         */
        template <class T>
        void publish(size_t channel_id, const T& event){
            std::type_index ind(typeid(T));
            channelentry ind_ev(ind, &event);
            _private_::g_channels[channel_id].push(ind_ev);
        }
        
        /**
         * @brief Free a channel and its identifier.
         * @details All subscribers are removed from the channel and the
         * identifier is made available to be reused. This should only be 
         * made after no listener expects more messages from this 
         * channel.
         * @param name Channel name.
         */
        bool freeChannel(const std::string& name);
        
        /**
         * @brief Delivers the events of every channel to its 
         * subscribers.
         * @details Dispatches every channel in the system.
         * @see dispatch(size_t)
         */
        void dispatch();
        
        /**
         * @brief Delivers the events of certain channel to its 
         * subscribers.
         * @details Before calling this function, the events are just 
         * queued. Afterwards, every subscriber is notified about each 
         * event in the channel and these are removed from it.
         * @param channel_id Identifier of the channel to dispatch.
         * @see EventListener::notify
         */
        void dispatch(size_t channel_id);
    }
}

#endif
