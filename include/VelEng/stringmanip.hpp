#ifndef __STRINGMANIP_HPP__
#define __STRINGMANIP_HPP__

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"

#include "VelEng/log.hpp"

namespace ge{ 
    void toLower(std::string& str);
    void ltrim(std::string& str, std::string totrim=" \t\n");
    void rtrim(std::string& str, std::string totrim=" \t\n");
    void trim(std::string& str, std::string totrim=" \t\n");
    std::vector<std::string> tokenize(const std::string& str, char del=' ');
    std::unordered_multimap<std::string, std::string> parseMap (const std::string& str, char pair_del=';', char kv_del='=', bool dotrim=true);
    template<typename T>
    sf::Vector2<T> parseVector2(std::string str){
        sf::Vector2<T> ret;
        ltrim(str," \t\n[");
        rtrim(str," \t\n]");
        std::stringstream ss(str);
        ss >> ret.x;
        ss.ignore(100,',');
        ss >> std::ws >> ret.y;
        CHECK_F(!ss.fail(), "Error parsing vector: {}", str);
        return ret;
    }
    template<typename T>
    sf::Rect<T> parseRect(std::string str){
        sf::Rect<T> ret;
        ltrim(str," \t\n[");
        rtrim(str," \t\n]");
        std::stringstream ss(str);
        ss >> ret.left;
        ss.ignore(100,',');
        ss >> std::ws >> ret.top;
        ss.ignore(100,',');
        ss >> std::ws >> ret.width;
        ss.ignore(100,',');
        ss >> std::ws >> ret.height;
        CHECK_F(!ss.fail(), "Error parsing rectangle: {}", str);
        return ret;
    }
}
#endif
