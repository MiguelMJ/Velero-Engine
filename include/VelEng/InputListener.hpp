#ifndef __INPUTLISTENER_HPP__
#define __INPUTLISTENER_HPP__

#include <set>

#include "SFML/Window/Event.hpp"

namespace ven{
    class InputListener{
    public:
        virtual void notify(sf::Event) = 0;
    };
}

#endif
