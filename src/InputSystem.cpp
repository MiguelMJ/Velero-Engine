#include "InputSystem.hpp"

#include <map>
#include <set>

namespace ge{
    namespace InputSystem{
        std::map<sf::Event::EventType, std::set<InputListener*> > g_listeners;
        void subscribe(sf::Event::EventType type, InputListener* listener){
            g_listeners[type].insert(listener);
        }
        void unsubscribe(sf::Event::EventType type, InputListener* listener){
            g_listeners[type].erase(listener);
        }
        void dispatch(sf::Event event){
            for(auto& l : g_listeners[event.type]){
                l->notify(event);
            }
        }
    }
}
