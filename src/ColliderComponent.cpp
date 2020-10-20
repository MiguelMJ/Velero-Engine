#include "VelEng/ColliderComponent.hpp"
#include "VelEng/CollisionEvent.hpp"
#include "VelEng/PhysicComponent.hpp"

#include "fmt/core.h"

#include "VelEng/vertexarray.hpp"
#include "VelEng/stringmanip.hpp"
#include "VelEng/Context.hpp"

namespace ven{
    Collider::Collider(){
        Component::m_eventsHandled.insert(typeid(CollisionEvent));
    }
    void Collider::handle(const ven::Event* e, std::type_index, size_t){
        auto ce = (const CollisionEvent*) e;
        if(ce->priority > m_priority && ce->solution.y < 0){
            Effect<Physic,int> eff = {
                0,
                [&](Physic& ph, const int&){
                    ph.m_velocity.y=0;
                }
            };
            getEntityPtr()->apply(eff);
        }
    }
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
        return "NOT IMPLEMENTED";
    }
    void Collider::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        t.draw(getLines(m_transformedPolygon, sf::Color::Cyan),s);
    }
    sf::FloatRect Collider::getGlobalBounds() const{
        return getLines(m_transformedPolygon).getBounds();
    }
    Component* parseCollider(const std::string& line){
        auto ret = new Collider;
        auto dict = parseMap(line);
        for(auto& kv : dict){
            if(kv.first == "point"){
                // DLOG_F(INFO"");
                ret->m_basePolygon.push_back(parseVector2<float>(kv.second));
            }else if(kv.first == "priority"){
                ret->m_priority = std::stoi(kv.second);
            }else if(kv.first == "solid"){
                ret->m_solid = kv.second == "on";
            }else{
                LOG_F(WARNING, "Unrecognized key for Collider: {}", kv.first);
            }
        }
        return ret;
    }
}
