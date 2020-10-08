#ifndef __COLLISIONEVENT_HPP__
#define __COLLISIONEVENT_HPP__

#include "Event.hpp"
#include "Entity.hpp"   

namespace ge{
    class CollisionEvent: public Event{
    public:
        Entity* entity;
        int priority;
        sf::Vector2f solution;
    };    
}

#endif