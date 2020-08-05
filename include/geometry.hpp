#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include <utility>
#include <array>
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Transform.hpp"

namespace ge{      
    static const float PI = 3.14159265f;
    static const float PI2 = PI*2;
    
    typedef std::pair<sf::Vector2f, sf::Vector2f> Segment;
    typedef std::vector<sf::Vector2f> Polygon;
    typedef struct{
        sf::Vector2f origin;
        sf::Vector2f direction;
        float angle; // rads
    } Ray;
    
    Ray make_ray(sf::Vector2f p1, sf::Vector2f p2);
    Ray make_ray(sf::Vector2f orig, float ang); // rads
    Ray make_ray(Segment s);
    
    std::array<Segment, 4> segments(sf::FloatRect rect);
    
    void transform(Polygon& polygon, const sf::Transform& transform);
    
    std::pair<float, float> intersection(Ray r1, Ray r2);
    std::pair<float, float> intersection(Segment r1, Segment r2);
    
    float getAngle(sf::Vector2f v);
    sf::Vector2f getVector(float ang); // rads // returns normalized
    sf::Vector2f normalize(sf::Vector2f v); // rads
    
    float length(sf::Vector2f v);
    float rad2deg(float rad);
    float deg2rad(float deg);
}

#endif
