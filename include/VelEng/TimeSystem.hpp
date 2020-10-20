/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the TimeSystem class.
 */
#ifndef __TIMESYSTEM_HPP__
#define __TIMESYSTEM_HPP__

#include <set>

#include "SFML/System/Time.hpp"

#include "VelEng/TimeSystem.hpp"
#include "VelEng/Updatable.hpp"
namespace ge{
    /**
     * @brief System to update the state of objects affected by time.
     */
    class TimeSystem final{
    private:
        std::set<Updatable*> m_updatables;
    public:
        /**
         * @brief Factor that multiplies the time applied to objects.
         * 1 by default.
         */
        float m_factor=1;
        
        /**
         * @brief Add an updatable object to the system.
         * @param updatable Pointer to the object.
         */
        void addUpdatable(Updatable* updatable);
        
        /**
         * @brief Remove an updatable object from the system.
         * @param updatable Pointer to the object.
         */
        void removeUpdatable(Updatable* updatable);
        
        /**
         * @brief Updates every object the time multiplied by the factor.
         * @see m_factor Updatable::update
         */
        void update(sf::Time delta);
    };
}

#endif
