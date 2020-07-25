/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the LightingLayer class.
 */
#ifndef __LIGHTINGLAYER_HPP__
#define __LIGHTINGLAYER_HPP__

#include <SFML/Graphics.hpp>

#include <set>

#include "Layer.hpp"
#include "LightSource.hpp"

namespace ge{
    /**
     * @brief %Layer for [light sources](@ref LightSource).
     * @details This kind of layer is not meant to be used with regular 
     * renderables, but with light sources. All functions that take a 
     * @ref Renderable as an argument, expect it to be a 
     * @ref LightSource. As a static cast is done at the beginning of 
     * each function, passing the wrong object may result in undefined 
     * behaviour.
     * 
     * Light has a more complex process of drawing because it takes two
     * steps. The first one is to reveal of the field of view, that is
     * showing the images below the light layer that are hidden by a more
     * or less opaque texture we call fog. The second one is propperly
     * lighting the image below the layer, in case that the light has a
     * defined color.
     */
    class LightingLayer: public Layer{
    private:
        std::set <const LightSource*> m_visibleLights;
        std::set <const LightSource*> m_invisibleLights;
        sf::RenderTexture m_fogTexture;
        sf::VertexArray m_fogQuad;
        sf::Vector2f m_fogOffset;
        sf::Color m_fogColor;
        // sf::Vector2f m_fog;
        
        /**
         * @copydoc Layer::draw
         */
        void draw(sf::RenderTarget&, sf::RenderStates)const override;
        
        /**
         * @brief This friendship is neccesary in order to
         * construct a static layer from a lighting one.
         */
        friend class StaticLayer;
    public:
        
        /**
         * @brief List of segments that cast shadows within this layer.
         * @details Every LightSource added to this layer will make its 
         * segment pool pointer point at this list.
         * @see LightSource::m_ptrSegmentPool
         */
        std::vector<Segment> m_segmentPool;
        
        /**
         * @brief Constructor.
         */
        LightingLayer();
        
        /**
         * @brief Copy constructor.
         * @param layer %Layer to copy.
         */
        LightingLayer(const LightingLayer& layer);
        
        /**
         * @brief Add a light source to the layer.
         * @param renderable Pointer to the LightSource.
         * 
         */
        void addRenderable(const Renderable* renderable) override;
        
        /**
         * @brief Remove a light source from the layer.
         * @param renderable Pointer to the LightSource.
         */
        void removeRenderable(const Renderable* renderable) override;
        
        /**
         * @copydoc Layer::clear
         */
        void clear() override;
        
        /**
         * @copydoc Layer::setVisible
         */
        void setVisible(const Renderable* renderable, bool visible) override;
        
        /**
         * @brief Set fog size.
         * @details Requires to update the fog.
         * @param width Fog width.
         * @param height Fog height.
         * @see adjustFog updateFog
         */
        void setFogSize(float width, float height);
        
        /**
         * @brief Set fog position.
         * @details Requires to update the fog.
         * @param x Fog position in horizontal axis.
         * @param y Fog position in vertical axis.
         * @see adjustFog updateFog
         */
        void setFogPosition(float x, float y);
        
        /**
         * @brief Set the fog position and size to match the view.
         * @details Requires to update the fog.
         * @param view View to adjust the fog.
         * @see updateFog setFogSize setFogPosition
         */
        void adjustFog(const sf::View& view);
        
        /**
         * @brief Set the fog position and size to match the view. 
         * @details Requires to update the fog
         * @param color Color to set the fog.
         * @see updateFog
         */
        void setFogColor(sf::Color color);
        
        /**
         * @brief Set the opacity of the fog.
         * @details Requires to update the fog.
         * @param color Value between 0 for total transparency and 1 for
         * total opacity.
         * @see updateFog
         */
        void setFogOpacity(float opacity);
        
        /**
         * @brief Applies fog changes and updates the field of view of 
         * the lights.
         * @details If the fog is visible and the lights move, this
         * function is required to keep the fog coherent with the lights.
         */
        void updateFog();
    };
}

#endif
