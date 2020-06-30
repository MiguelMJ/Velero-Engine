#ifndef __RENDERABLE_HPP__
#define __RENDERABLE_HPP__

#include "SFML/Graphics.hpp"

namespace ge{
    class Renderable: public sf::Drawable, public sf::Transformable{
    private:
        size_t m_layer;
        bool m_visible;
        void draw(sf::RenderTarget& t, sf::RenderStates s) const = 0;
    public:
        virtual ~Renderable(){}
        virtual sf::FloatRect getGlobalBounds() const = 0;
    };
}

#endif
