#ifndef __LIGHTSOURCE_HPP__
#define __LIGHTSOURCE_HPP__

#include <cmath>
#include <vector>
#include <algorithm>

#include "SFML/Graphics.hpp"

#include "Renderable.hpp"
#include "geutil.hpp"

namespace ge{
    class LightSource: public Renderable{
    private:
        sf::VertexArray m_polygon;
        sf::FloatRect m_bounds; 
        sf::Color m_color;
        std::vector<Segment>* m_ptrSegmentPool;
        bool m_glow;
        friend class LightingLayer;
        void draw(sf::RenderTarget&, sf::RenderStates) const override;
    public:
        LightSource();
        virtual ~LightSource();
        
        sf::FloatRect getGlobalBounds() const override;
        
        void setIntensity(float);
        void setColor(sf::Color);
        void setRadius(float);
        void setGlow(bool);
        
        void castLight();
    };
}

#endif
