#ifndef __COLLISIONEVENT_HPP__
#define __COLLISIONEVENT_HPP__

#include "VelEng/Event.hpp"
#include "VelEng/Entity.hpp"   

namespace ven{
    class CollisionEvent: public Event{
    public:
        Entity* entity;
        int priority;
        sf::Vector2f solution;
    };    
}

#endif
