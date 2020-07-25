#ifndef __VERTEXARRAY_HPP__
#define __VERTEXARRAY_HPP__

#include "SFML/Graphics.hpp"

namespace ge{
    sf::VertexArray getVertex(sf::FloatRect rect, sf::Color c=sf::Color::White);
    void setColor(sf::VertexArray& va, sf::Color);
    void setColor(sf::VertexArray& va, sf::Color, int first, int size);
    void move(sf::VertexArray& va, sf::Vector2f);
}

#endif
