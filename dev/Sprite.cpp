#include "Sprite.hpp"

namespace ge{
    Sprite::Sprite()
    : m_quad(sf::Quads, 4)
    {
        m_quad[1].position = {0, 10};
        m_quad[2].position = {10, 10};
        m_quad[3].position = {10, 0};
    }
    sf::FloatRect Sprite::getGlobalBounds() const{
        return Transformable::getTransform().transformRect(m_quad.getBounds());
    }
    void Sprite::setColor(sf::Color c){
        ge::setColor(m_quad, c);
    }
    void Sprite::draw(sf::RenderTarget& t, sf::RenderStates s)const{
        s.transform *= Transformable::getTransform();
        t.draw(m_quad, s);
    }
}
