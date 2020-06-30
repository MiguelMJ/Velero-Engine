#include "geutil.hpp"

static const float PI = 3.14159265f;
static const float PI2 = PI*2;

namespace ge{
    std::array<Segment, 4> segmentsFromRect(sf::FloatRect rect){
        sf::Vector2f tl(rect.left, rect.top);
        sf::Vector2f tr(rect.left + rect.width, rect.top);
        sf::Vector2f br(rect.left + rect.width, rect.top + rect.height);
        sf::Vector2f bl(rect.left, rect.top + rect.height);
        Segment top = {tl,tr};
        Segment right = {tr,br};
        Segment bottom = {br,bl};
        Segment left = {bl,tl};
        return {
            top,
            right,
            bottom,
            left
        };
    }
    Ray make_ray(sf::Vector2f p1, sf::Vector2f p2){
        sf::Vector2f d = p2 - p1;
        return {
            p1,
            d,
            std::atan2(d.y, d.x)
        };
    }
    Ray make_ray(sf::Vector2f orig, float ang){ // rads
        ang = (float)fmod(ang + PI , PI2);
        if(ang < 0) ang += PI2;
        ang -= PI;
        return {
            orig,
            sf::Vector2f(std::cos(ang), std::sin(ang)),
            ang
        };
    }
    Ray make_ray(Segment s){
        return make_ray(s.first, s.second);
    }
    std::pair<float, float> intersection(Ray r1, Ray r2){
        float t1, t2;
        t1 = t2 = std::numeric_limits<float>::infinity();
        sf::Vector2f dir1 (r1.direction), dir2 (r2.direction);
        sf::Vector2f orig1 (r1.origin), orig2 (r2.origin);
        if(dir1.x/dir2.x != dir1.y/dir2.y){
            t2 = (dir1.x * (orig2.y - orig1.y) + dir1.y * (orig1.x - orig2.x)) /
                 (dir2.x * dir1.y - dir2.y * dir1.x);
            t1 = (orig2.x + dir2.x * t2 - orig1.x) / dir1.x;
        }
        return std::make_pair(t1, t2);
    }
    float rad2deg(float rad){
        return rad * 180 / PI;
    }
    float deg2rad(float deg){
        return deg * PI / 180;
    }
    sf::VertexArray vertexRect(sf::FloatRect rect, sf::Color c){
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
            va[i].position.x += d.x;
            va[i].position.y += d.y;
        }
    }
}
