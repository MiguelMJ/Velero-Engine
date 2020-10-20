/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the InputSystem namespace.
 */
#ifndef __INPUTSYSTEM_HPP__
#define __INPUTSYSTEM_HPP__

#include "SFML/Window/Event.hpp"
#include "VelEng/InputListener.hpp"

namespace ge{
    /**
     * @brief System for objects to listen to user input.
     */
    namespace InputSystem{
        void subscribe(sf::Event::EventType type, InputListener* listener);
        void unsubscribe(sf::Event::EventType type, InputListener* listener);
        void dispatch(sf::Event event);
    }
}

#endif
