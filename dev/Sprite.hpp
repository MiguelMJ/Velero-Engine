#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__



#include "SFML/Graphics.hpp"

#include "Renderable.hpp"
#include "geutil.hpp"

namespace ge{
    class Sprite: public Renderable{
    private:
        sf::VertexArray m_quad;
        void draw(sf::RenderTarget& t, sf::RenderStates) const override;
    public:
        Sprite(sf::Color c=sf::Color::White);
        sf::FloatRect getGlobalBounds() const override;
        void setColor(sf::Color);
    };
}

#endif
