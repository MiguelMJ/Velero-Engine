#ifndef __GEUTIL_HPP__
#define __GEUTIL_HPP__

#include <iostream>
#define lll(x) \
    std::cout << x << std::endl

#include <utility>
#include <cmath>
#include <limits>
#include <array>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

namespace ge{
    // geometry
    typedef std::pair<sf::Vector2f, sf::Vector2f> Segment;
    std::array<Segment, 4> segmentsFromRect(sf::FloatRect rect);
    typedef struct{
        sf::Vector2f origin;
        sf::Vector2f direction;
        float angle; // rads
    } Ray;
    Ray make_ray(sf::Vector2f p1, sf::Vector2f p2);
    Ray make_ray(sf::Vector2f orig, float ang); // rads
    Ray make_ray(Segment s);
    std::pair<float, float> intersection(Ray r1, Ray r2);
    float rad2deg(float rad);
    float deg2rad(float deg);
    
    // work with VertexArray
    sf::VertexArray getVertex(sf::FloatRect rect, sf::Color c=sf::Color::White);
    void setColor(sf::VertexArray& va, sf::Color);
    void setColor(sf::VertexArray& va, sf::Color, int first, int size);
}

#endif
