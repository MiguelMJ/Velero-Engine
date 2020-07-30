/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the Updatable interface.
 */
#ifndef __UPDATABLE_HPP__
#define __UPDATABLE_HPP__

#include "SFML/System/Time.hpp"

namespace ge{
    /**
     * @brief Abstract base class for objects managed by 
     * [time systems](@ref TimeSystem)
     */
    class Updatable{
    public:
        /**
         * @brief Updates the object state for a delta time.
         * @param delta Time passed since last update.
         */
        virtual void update (sf::Time delta) = 0;
    };
}

#endif
