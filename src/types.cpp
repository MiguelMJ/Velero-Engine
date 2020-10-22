#include "VelEng/types.hpp"
#include "VelEng/stringmanip.hpp"
#include "fmt/core.h"
#include "VelEng/log.hpp"

#include <stdexcept>
#include <string>
#include <algorithm>
#include <sstream>

namespace ven{
    typedef struct {float x; float y;} v2;
    v2 parseV2(const rapidjson::Value& json){
        auto arr = json.GetArray();
        CHECK_F(arr.Size() == 2,
                "2D vector must be size 2");
        return {arr[0].GetFloat(),arr[1].GetFloat()};
    }
    
    // 2D Vectors
    sf::Vector2f parseSFMLVector(const rapidjson::Value& json){
        v2 aux = parseV2(json);
        sf::Vector2f vec(aux.x, aux.y);
        return vec;
    }
    b2Vec2 parseB2Vector(const rapidjson::Value& json){
        v2 aux = parseV2(json);
        b2Vec2 vec(aux.x, -aux.y);
        return vec;
    }
    b2Vec2 SFMLtoB2(const sf::Vector2f& v){
        return b2Vec2(v.x, -v.y);
    }
    sf::Vector2f B2toSFML(const b2Vec2& v){
        return sf::Vector2f(v.x, -v.y);
    }
    std::string to_string(const sf::Vector2f v){
        return fmt::format("[{},{}]",v.x, v.y);
    }
    std::string to_string(const b2Vec2 v){
        return fmt::format("[{},{}]",v.x, v.y);
    }
    
    // Polygon
    sf::VertexArray parseSFMLPolygon(const rapidjson::Value& json, bool fill){
        auto arr = json.GetArray();
        sf::VertexArray polygon(fill?sf::TriangleFan : sf::LineStrip, 1);
        for(auto& vec: arr){
            polygon.append(sf::Vertex(parseSFMLVector(vec)));
        }
        if(fill){
            for(size_t i=1; i < polygon.getVertexCount(); i++){
                polygon[0].position += polygon[i].position;
            }
            polygon[0].position *= 1.0f/(polygon.getVertexCount()-1);
        }else{
            polygon[0].position = polygon[polygon.getVertexCount()-1].position;
        }
        return polygon;
    }
    b2PolygonShape parseB2Polygon(const rapidjson::Value& json){
        auto arr = json.GetArray();
        b2PolygonShape polygon;
        b2Vec2 points[b2_maxPolygonVertices];
        for(size_t i=0; i < arr.Size(); i++){
            points[i] = parseB2Vector(arr[i]);
        }
        polygon.Set(points, arr.Size());
        return polygon;
        
    }
    sf::VertexArray B2toSFML(const b2PolygonShape& poly, bool fill){
        sf::VertexArray result(fill? sf::TriangleFan: sf::LineStrip, poly.m_count+1);
        for(int i=0; i < poly.m_count; i++){
            result[i+1].position = B2toSFML(poly.m_vertices[i]);
        }
        if(fill){
            for(int i=1; i <= poly.m_count; i++){
                result[0].position += result[i].position;
            }
            result[0].position *= 1.0f/(poly.m_count);
        }else{
            result[0].position = result[poly.m_count].position;
        }
        return result;
    }
    b2PolygonShape SFMLtoB2(const sf::VertexArray& poly){
        b2PolygonShape result;
        b2Vec2 points[b2_maxPolygonVertices];
        for(size_t i=0; i < poly.getVertexCount(); i++){
            points[i] = SFMLtoB2(poly[i].position);
        }
        result.Set(points, poly.getVertexCount());
        return result;
    }
    std::string to_string(const b2PolygonShape& poly){
        std::stringstream ss;
        ss << "[";
        if(poly.m_count > 0){
            ss << ',' << to_string(poly.m_vertices[0]);
            for(int i=1; i < poly.m_count; i++){
                ss << ',' << to_string(poly.m_vertices[i]);
            }
        }
        ss << ']';
        return ss.str();
    }
    std::string to_string(const sf::VertexArray& poly){
        std::stringstream ss;
        ss << "[";
        if(poly.getVertexCount() > 0){
            ss << ',' << to_string(poly[0].position);
            for(size_t i=1; i < poly.getVertexCount(); i++){
                ss << ',' << to_string(poly[i].position);
            }
        }
        ss << ']';
        return ss.str();
    }
}
