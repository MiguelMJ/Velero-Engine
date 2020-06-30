#ifndef __STATICLAYER_HPP__
#define __STATICLAYER_HPP__

#include <cmath>

#include "SFML/Graphics.hpp"

#include "Layer.hpp"

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
        void addRenderable(const Renderable*) override;
        void setVisible(const Renderable*, bool) ;
        void display();
    };
}

#endif
