#ifndef __LAYER_HPP__
#define __LAYER_HPP__

#include "SFML/Graphics.hpp"

#include "Renderable.hpp"

namespace ge{
    class Layer: public sf::Drawable{
    private:
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
    public:
        virtual ~Layer(){}
        virtual void addRenderable(const Renderable*) = 0;
        virtual void setVisible(const Renderable*, bool) = 0;
    };
}

#endif
