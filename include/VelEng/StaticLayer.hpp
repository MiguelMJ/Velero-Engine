/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the StaticLayer class.
 */
#ifndef __STATICLAYER_HPP__
#define __STATICLAYER_HPP__

#include <cmath>
#include <limits>

#include "SFML/Graphics.hpp"

#include "VelEng/Layer.hpp"
#include "VelEng/DynamicLayer.hpp"
#include "VelEng/LightingLayer.hpp"

namespace ven{
    /**
     * @brief Layer that doesn't allow changes.
     * @details This layer buffers the renderables added to it, and 
     * requires a manual update to show them. It is thought to be updated
     * once and not to be changed after that. If you wanted to make a 
     * second update and keep the initial renderables, they would have to 
     * be added again.
     * 
     * This layer is meant for grouping objects that won't be updating  
     * and keep the draw time constant, no matter the number of 
     * objects.
     */
    class StaticLayer: public Layer{
    private:
        sf::RenderTexture m_renderTexture;
        sf::Vector2f m_offset;
        sf::VertexArray m_quad;
        std::vector<const Renderable*> m_unrendered;
        /**
         * @copydoc Layer::draw
         */
        void draw(sf::RenderTarget& t, sf::RenderStates) const override;
    public:
        
        /**
         * @brief Construct empty layer.
         */
        StaticLayer();
        
        /**
         * @brief Copy constructor.
         * @details The layer is automatically updated, so there is no 
         * need of calling @ref display.
         * @param layer %Layer to copy.
         */
        StaticLayer(const StaticLayer& layer);
        
        /**
         * @brief Construct from a dynamic layer.
         * @details The layer is automatically updated, so there is no 
         * need of calling @ref display.
         * @param layer %Layer to copy.
         */
        StaticLayer(const DynamicLayer& layer);
        
        /**
         * @brief Construct from lighting layer.
         * @details The layer is automatically updated, so there is no 
         * need of calling @ref display.
         * @param layer %Layer to copy.
         */
        StaticLayer(const LightingLayer& layer);
        
        /**
         * @brief Add renderable to the render buffer.
         * @see display
         * @param renderable Pointer to renderable to add.
         */
        void addRenderable(const Renderable* renderable) override;
        
        /**
         * @brief Does nothing.
         * @details This function exists because it's inherited.
         * @param renderable Ignored pointer.
         */
        void removeRenderable(const Renderable* renderable) override;
        
        /**
         * @copydoc Layer::clear
         */
        void clear() override;
        
        /**
         * @brief Updates the layer with the buffered renderables.
         * @details The layer displays the renderables added and clears 
         * the buffer. This function should not be called after 
         * constructing the static layer from other layer, as it would 
         * reset the copy.
         * @see addRenderable
         */
        void display();
    };
}

#endif
