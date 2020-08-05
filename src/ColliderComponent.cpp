#include "ColliderComponent.hpp"

#include "fmt/core.h"

#include "stringmanip.hpp"
#include "Context.hpp"

namespace ge{
    Component* Collider::copy() const{
        return new Collider(*this);
    }
    void Collider::onActivate(){
        M_CS.addCollider(this);
    }
    void Collider::onDeactivate(){
        M_CS.removeCollider(this);
    }
    std::string Collider::to_string() const {
        std::stringstream ss;
        ss << "collider weight = " << m_priority;
        for(auto& p : m_basePolygon){
            ss << fmt::format("; p = {{{},{}}}", p.x, p.y);
        }
        return ss.str();
    }
    Component* parseCollider(std::istream& in){
        auto ret = new Collider;
        std::string line;
        getline(in, line);
        auto dict = parseMap(line);
        for(auto& kv : dict){
            if(kv.first == "point"){
                ret->m_basePolygon.push_back(parseVector2<float>(kv.second));
            }else if(kv.first == "weight"){
                ret->m_priority = std::stoi(kv.second);
            }
        }
        return ret;
    }
}
