#ifndef __RENDERABLE_HPP__
#define __RENDERABLE_HPP__

#include "SFML/Graphics.hpp"

namespace ge{
    class Renderable: public sf::Drawable, public sf::Transformable{
    private:
        virtual void draw(sf::RenderTarget& t, sf::RenderStates s) const override = 0;
        friend class Layer;
    public:
        virtual ~Renderable(){}
        virtual sf::FloatRect getGlobalBounds() const = 0;
    };
}

#endif
