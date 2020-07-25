#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include "SFML/Graphics.hpp"
#include <utility>

namespace ge{      
    static const float PI = 3.14159265f;
    static const float PI2 = PI*2;
    
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
}

#endif
