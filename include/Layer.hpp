/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the Layer abstract class.
 */
#ifndef __LAYER_HPP__
#define __LAYER_HPP__

#include "SFML/Graphics.hpp"

#include "Renderable.hpp"

namespace ge{
    /**
     * @brief Abstract base class for containers of 
     * [renderables](@ref Renderable) for the
     * [render system](@ref RenderSystem).
     */
    class Layer: public sf::Drawable{
    private:
        /**
         * @brief Draws all the renderables contained by the layer.
         * @param target Render target.
         * @param states Current render states.
         */
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    public:
        virtual ~Layer(){}
        /**
         * @brief Add a renderable to the layer.
         * @param renderable Pointer to the renderable object.
         */
        virtual void addRenderable(const Renderable* renderable) = 0;
        /**
         * @brief Remove a renderable from the layer.
         * @param renderable Pointer to the renderable object.
         */
        virtual void removeRenderable(const Renderable* renderable) = 0;
        /**
         * @brief Remove all the renderables from the layer.
         */
        virtual void clear() = 0;
    };
}

#endif
