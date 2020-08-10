#include "vertexarray.hpp"

#include "log.hpp"
#include "fmt/ranges.h"

namespace ge{
    sf::VertexArray getLines(const Polygon& poly, sf::Color c){
        sf::VertexArray ret(sf::LineStrip, poly.size()+1);
        for(unsigned i = 0; i < poly.size(); i++){
            ret[i].position = poly[i];
            ret[i].color = c;
        }
        ret[poly.size()] = ret[0];
        return ret;
    }
    sf::VertexArray getLines(sf::FloatRect rect, sf::Color c){
        sf::VertexArray ret(sf::LineStrip, 5);
        ret[0].position = {rect.left, rect.top};
        ret[1].position = {rect.left + rect.width, rect.top};
        ret[2].position = {rect.left + rect.width, rect.top + rect.height};
        ret[3].position = {rect.left, rect.top + rect.height};
        ret[4].position = ret[0].position;
        for(int i=0; i < 5; i++){
            ret[i].color = c;
        }
        return ret;
    }
    sf::VertexArray getQuad(sf::FloatRect rect, sf::Color c){
        sf::VertexArray ret(sf::Quads, 5);
        ret[0].position = {rect.left, rect.top};
        ret[1].position = {rect.left + rect.width, rect.top};
        ret[2].position = {rect.left + rect.width, rect.top + rect.height};
        ret[3].position = {rect.left, rect.top + rect.height};
        for(int i=0; i < 5; i++){
            ret[i].color = c;
        }
        return ret;
    }
    void setTextureRect(sf::VertexArray& va, sf::FloatRect rect, int pos){
        va[pos+0].texCoords = {rect.left, rect.top};
        va[pos+1].texCoords = {rect.left + rect.width, rect.top};
        va[pos+2].texCoords = {rect.left + rect.width, rect.top + rect.height};
        va[pos+3].texCoords = {rect.left, rect.top + rect.height};
    }
    void setColor(sf::VertexArray& va, sf::Color c){
        for(unsigned i = 0; i < va.getVertexCount(); i++){
            va[i].color = c;
        }
    }
    void setColor(sf::VertexArray& va, sf::Color c, int first, int size){
        for(int i=first; i < first + size; i++){
            va[i].color = c;
        }
    }
    void move(sf::VertexArray& va, sf::Vector2f d){
        for(unsigned i=0; i < va.getVertexCount(); i++){
            va[i].position.y += d.y;
            va[i].position.x += d.x;
        }
    }
}
