#include "ColliderComponent.hpp"

#include "fmt/core.h"

#include "vertexarray.hpp"
#include "stringmanip.hpp"
#include "Context.hpp"

namespace ge{
    Component* Collider::copy() const{
        return new Collider(*this);
    }
    void Collider::onActivate(){
        DLOG_F(INFO,"Collider activated");
        M_RSD.getLayer("debug")->addRenderable(this);
        M_CS.addCollider(this);
    }
    void Collider::onDeactivate(){
        DLOG_F(INFO,"Collider deactivated");
        M_RSD.getLayer("debug")->removeRenderable(this);
        M_CS.removeCollider(this);
    }
    std::string Collider::to_string() const{
        std::stringstream ss;
        ss << "collider weight = " << m_priority;
        for(auto& p : m_basePolygon){
            ss << fmt::format("; point = {{{},{}}}", p.x, p.y);
        }
        return ss.str();
    }
    void Collider::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        t.draw(getLines(m_transformedPolygon, sf::Color::Cyan),s);
    }
    sf::FloatRect Collider::getGlobalBounds() const{
        return getLines(m_transformedPolygon).getBounds();
    }
    Component* parseCollider(std::istream& in){
        auto ret = new Collider;
        std::string line;
        getline(in, line);
        auto dict = parseMap(line);
        for(auto& kv : dict){
            if(kv.first == "point"){
                // DLOG_F(INFO"");
                ret->m_basePolygon.push_back(parseVector2<float>(kv.second));
            }else if(kv.first == "weight"){
                ret->m_priority = std::stoi(kv.second);
            }else{
                LOG_F(WARNING, "Unrecognized key for Collider: {}", kv.first);
            }
        }
        return ret;
    }
}
