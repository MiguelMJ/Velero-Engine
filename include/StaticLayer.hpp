#ifndef __STATICLAYER_HPP__
#define __STATICLAYER_HPP__

#include <cmath>
#include <limits>

#include "SFML/Graphics.hpp"

#include "Layer.hpp"
#include "DynamicLayer.hpp"
#include "LightingLayer.hpp"

namespace ge{
    class StaticLayer: public Layer{
    private:
        sf::RenderTexture m_renderTexture;
        sf::Vector2f m_offset;
        sf::VertexArray m_quad;
        std::vector<const Renderable*> m_unrendered;
        void draw(sf::RenderTarget& t, sf::RenderStates) const override;
    public:
        StaticLayer();
        StaticLayer(const StaticLayer&);
        StaticLayer(const DynamicLayer&);
        StaticLayer(const LightingLayer&);
        void addRenderable(const Renderable*) override;
        void setVisible(const Renderable*, bool) ;
        void display();
    };
}

#endif
