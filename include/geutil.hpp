#ifndef __GEUTIL_HPP__
#define __GEUTIL_HPP__

#include <algorithm>
#include <cctype>
#include <utility>
#include <cmath>
#include <limits>
#include <array>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "loguru.hpp"

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
    float getAngle(sf::Vector2f v);
    sf::Vector2f getVector(float ang); // rads
    sf::Vector2f normalize(sf::Vector2f v); // rads
    float length(sf::Vector2f v);
    float rad2deg(float rad);
    float deg2rad(float deg);
    
    // work with VertexArray
    sf::VertexArray getVertex(sf::FloatRect rect, sf::Color c=sf::Color::White);
    void setColor(sf::VertexArray& va, sf::Color);
    void setColor(sf::VertexArray& va, sf::Color, int first, int size);
    void move(sf::VertexArray& va, sf::Vector2f);
    
    // work with strings
    void toLower(std::string& str);
}

#endif
