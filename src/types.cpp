#include "types.hpp"
#include "stringmanip.hpp"
#include "fmt/core.h"

#include <stdexcept>
#include <regex>
#include <string>
#include <algorithm>
#include <sstream>

namespace ge{
    const std::string ws_str("\\w*");
    const std::string float_str("d+(\\.\\d*)?");
    const std::string v2_str(fmt::format("\\[{0}{1}{0},{0}{1}{0}\\]",ws_str,float_str));
    const std::string polygon_str(fmt::format("\\[({0}{1}({0},{0}{1})*)?{0}\\]",ws_str,v2_str));
    const std::regex ws_re(ws_str);
    const std::regex float_re(float_str);
    const std::regex v2_re(v2_str);
    const std::regex polygon_re(polygon_str);
    typedef struct {float x; float y;} v2;
    v2 parseV2(const std::string& str){
        if(!std::regex_match(str, v2_re)) 
            throw std::invalid_argument(fmt::format("{} can't be parsed as a 2D vector",str));
        std::sregex_iterator i = std::sregex_iterator(str.begin(), str.end(), float_re);
        return {std::stof(i->str()), std::stof((++i)->str())};
    }
    
    // 2D Vectors
    sf::Vector2f parseSFMLVector(const std::string& str){
        v2 aux = parseV2(str);
        sf::Vector2f vec(aux.x, aux.y);
        return vec;
    }
    b2Vec2 parseB2Vector(const std::string& str){
        v2 aux = parseV2(str);
        b2Vec2 vec(aux.x, aux.y);
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
    sf::VertexArray parseSFMLPolygon(const std::string& str, bool fill){
        if(!std::regex_match(str, polygon_re))
            throw std::invalid_argument(fmt::format("{} can't be parsed as a polygon using {}",str,polygon_str));
        sf::VertexArray polygon(fill?sf::TriangleFan : sf::LineStrip, 1);
        std::sregex_iterator it(str.begin(), str.end(), v2_re);
        std::sregex_iterator end;
        while(it != end){
            polygon.append(sf::Vertex(parseSFMLVector(it->str())));
            it++;
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
    b2PolygonShape parseB2Polygon(const std::string& str){
        if(!std::regex_match(str, polygon_re))
            throw std::invalid_argument(fmt::format("{} can't be parsed as a polygon using {}",str,polygon_str));
        b2PolygonShape polygon;
        int32 count = (std::count(str.begin(), str.end(), ',')+1)/2;
        b2Vec2 points[b2_maxPolygonVertices];
        int i = 0;
        std::sregex_iterator it(str.begin(), str.end(), v2_re);
        std::sregex_iterator end;
        while(it != end){
            points[i++] = parseB2Vector(it->str());
            it++;
        }
        if(i != count) throw std::logic_error(fmt::format("Uexpected error counting elements in polygon from {}", str));
        polygon.Set(points, i);
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
    
    // Dictionary
    typedef std::map<std::string, std::string> Dictionary;
    Dictionary parseDictionary(const std::string& str, char pair_del, char kv_del, bool do_trim){
        Dictionary result;
        auto pairs = tokenize(str, pair_del);
        for(auto& pair: pairs){
            std::string key, val;
            std::stringstream ss(pair);
            getline(ss, key, kv_del);
            getline(ss, val);
            if(do_trim){
                trim(key);
                trim(val);
            }
            result.insert({key,val});
        }
        return result;
    }
}
