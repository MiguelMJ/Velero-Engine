#ifndef __VERTEXARRAY_HPP__
#define __VERTEXARRAY_HPP__

#include "SFML/Graphics.hpp"

#include "VelEng/geometry.hpp"

namespace ge{
    sf::VertexArray getLines(const Polygon& poly, sf::Color c=sf::Color::White);
    sf::VertexArray getLines(sf::FloatRect rect, sf::Color c=sf::Color::White);
    sf::VertexArray getQuad(sf::FloatRect rect, sf::Color c=sf::Color::White);
    void setTextureRect(sf::VertexArray& va, sf::FloatRect rect, int pos=0);
    void setColor(sf::VertexArray& va, sf::Color);
    void setColor(sf::VertexArray& va, sf::Color, int first, int size);
    void move(sf::VertexArray& va, sf::Vector2f);
}

#endif
