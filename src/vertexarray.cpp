#include "vertexarray.hpp"

namespace ge::util{
    sf::VertexArray getVertex(sf::FloatRect rect, sf::Color c){
        sf::VertexArray ret(sf::LineStrip, 5);
        ret[0].position = {rect.left, rect.top};
        ret[1].position = {rect.left, rect.top + rect.height};
        ret[2].position = {rect.left + rect.width, rect.top + rect.height};
        ret[3].position = {rect.left + rect.width, rect.top};
        ret[4].position = ret[0].position;
        for(int i=0; i < 5; i++){
            ret[i].color = c;
        }
        return ret;
    }
    void setColor(sf::VertexArray& va, sf::Color c){
        for(int i = 0; i < va.getVertexCount(); i++){
            va[i].color = c;
        }
    }
    void setColor(sf::VertexArray& va, sf::Color c, int first, int size){
        for(int i=first; i < first + size; i++){
            va[i].color = c;
        }
    }
    void move(sf::VertexArray& va, sf::Vector2f d){
        for(int i=0; i < va.getVertexCount(); i++){
            va[i].position.y += d.y;
            va[i].position.x += d.x;
        }
    }
}
