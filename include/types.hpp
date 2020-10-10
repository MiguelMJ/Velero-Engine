#ifndef __VELERO_TYPES_HPP__
#define __VELERO_TYPES_HPP__

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "box2d/b2_math.h"
#include "box2d/b2_polygon_shape.h"

#include <map>
#include <string>

namespace ge{
    // 2D Vectors
    sf::Vector2f parseSFMLVector(const std::string& str);
    b2Vec2 parseB2Vector(const std::string& str);
    sf::Vector2f B2toSFML(const b2Vec2&);
    b2Vec2 SFMLtoB2(const sf::Vector2f&);
    std::string to_string(const sf::Vector2f v);
    std::string to_string(const b2Vec2 v);
    
    // Polygon
    sf::VertexArray parseSFMLPolygon(const std::string& str, bool fill=false);
    b2PolygonShape parseB2Polygon(const std::string& str);
    sf::VertexArray B2toSFML(const b2PolygonShape& poly, bool fill=false);
    b2PolygonShape SFMLtoB2(const sf::VertexArray& poly);
    std::string to_string(const b2PolygonShape& poly);
    std::string to_string(const sf::VertexArray& poly);
    
    // Dictionary
    typedef std::map<std::string, std::string> Dictionary;
    Dictionary parseDictionary(const std::string& str, char pair_del=';', char kv_del='=', char do_trim=true);
}

#endif
