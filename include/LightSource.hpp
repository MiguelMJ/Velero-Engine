#ifndef __LIGHTSOURCE_HPP__
#define __LIGHTSOURCE_HPP__

#include <cmath>
#include <vector>
#include <algorithm>

#include "SFML/Graphics.hpp"

#include "Renderable.hpp"
#include "geutil.hpp"

namespace ge{
    /**
     * @brief Object that emits light within a 
     * [lighting layer](@ref LightingLayer).
     * @details These objects are meant to be used within a 
     * LightingLayer. They can also be used within a regular layer, but 
     * the lighting effect would have some limitations.
     */
    class LightSource: public Renderable{
    private:
        sf::VertexArray m_polygon;
        sf::FloatRect m_bounds; 
        sf::Color m_color;
        bool m_glow;
        
        /**
         * @brief Pointer to the list of segments that cast shadows.
         * @details By default, it points to @ref s_defaultSegmentPool.
         * @see s_defaultSegmentPool
         */
        std::vector<Segment>* m_ptrSegmentPool;
        
        /**
         * @brief This friendship is necessary to change the pointer of the 
         * segment pool
         * @see m_ptrSegmentPool LightingLayer::m_segmentPool
         */
        friend class LightingLayer;
        
        /**
         * @copydoc Renderable::draw
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    public:
        
        /**
         * @brief Default segment pool for shadow casting.
         * @see m_ptrSegmentPool
         */
        static std::vector<Segment> s_defaultSegmentPool;
        
        /**
         * @brief Constructor.
         */
        LightSource();
        virtual ~LightSource();
        
        /**
         * @copydoc Renderable::getGlobalBounds
         */
        sf::FloatRect getGlobalBounds() const override;
        
        /**
         * @brief Set the light intensity.
         * @details The @p intensity of the light determines two things: 
         * how much fog opacity it reduces, and how much of its color is 
         * added to the layers below when the _glow_ is active.
         * @param intensity Value from 0 to 1. At 0 the light is
         * invisible.
         * @see setGlow
         */
        void setIntensity(float intensity);
        
        /**
         * @brief Set the light color.
         * @details The light will only show color if the _glow_ is 
         * active.
         * @param color New color of the light.
         * @see setGlow
         */
        void setColor(sf::Color color);
        
        /**
         * @brief Set the radius of the iluminated area.
         * @param radius New radius of the area.
         */
        void setRadius(float radius);
        
        /**
         * @brief Set the value of the _glow_ flag.
         * @details When the @p glow is inactive, the @ref LightingLayer 
         * will only use the light to reveal the area under the fog. If 
         * it is active, it will also add the color to the image. Note 
         * that when the glow is not set, the light won't be visible if 
         * the [fog opacity](@ref LightingLayer::setFogOpacity) is 0.
         * @htmlonly <style> img[src="lightexample.png"]{width:300px;} 
         * </style> @endhtmlonly
         * @image html lightexample.png "Left: glow deactivated with intensity at 1 and 0.5. Right: glow activated with intensity at 1 and 0.5."
         * @param glow Value to set the flag.
         */
        void setGlow(bool glow);
        
        /**
         * @brief Calculates the area that should be iluminated with a 
         * ray casting algorithm.
         * @details For the calculations, the segments from @ref 
         * m_ptrSegmentPool are used. If the [fog opacity](@ref 
         * LightingLayer::setFogOpacity) is not 0, then @ref 
         * LightingLayer::updateFog should be called somewhere between 
         * this function and the next draw.
         * @see m_ptrSegmentPool
         */
        void castLight();
    };
}

#endif
