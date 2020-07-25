/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the Renderable
 * interface.
 */
#ifndef __RENDERABLE_HPP__
#define __RENDERABLE_HPP__

#include "SFML/Graphics.hpp"

namespace ge{
    /**
     * @brief Abstract base class for objects that  can be drawn in the [render system](@ref RenderSystem).
     * @details This class extends 
     * [sf::Drawable](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Drawable.php)
     * and [sf::Transformable](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Transformable.php)
     * from the SFML library.
     * 
     * To see the full set of functions of these parent classes, visit the SFML documentation.
     */
    class Renderable: public sf::Drawable, public sf::Transformable{
    private:
        /**
         * @brief Draw the object to a render target.
         * @details This function is inherited from sf::Drawable.
         * @param target Render target.
         * @param states Current render states.
         */
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
        friend class Layer;
    public:
        virtual ~Renderable(){}
        /**
         * @brief Get the bounds of the drawing in world coordinates.
         * @returns The global bounds of the object.
         */
        virtual sf::FloatRect getGlobalBounds() const = 0;
    };
}

#endif
