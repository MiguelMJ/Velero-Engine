#include "PhysicComponent.hpp"
#include "CollisionEvent.hpp"
#include "Context.hpp"
#include "stringmanip.hpp"

namespace ge{
    float Physic::s_meter(64);
    sf::Vector2f Physic::s_gravity(0,9.1);
    Physic::Physic(){
        Component::m_eventsHandled.insert(typeid(CollisionEvent));
    }
    void Physic::onActivate(){
        M_LTS.addUpdatable(this);
    }
    void Physic::onDeactivate(){
        M_LTS.removeUpdatable(this);
    }
    void Physic::handle(const Event* ev, std::type_index type, size_t){
        if(type == typeid(CollisionEvent)){
            const CollisionEvent* cev = (const CollisionEvent*) ev;
            Effect<Physic, float[3]> eff = {
                {m_mass, m_velocity.x, m_velocity.y},
                [&](Physic& ph, const float (&info)[3]){
                    ph.m_acceleration += info[0] * sf::Vector2f(info[1],info[2]) / ph.m_mass;
                }
            };
            cev->entity->apply(eff);
        }
    }
    Component* Physic::copy() const{
        return new Physic(*this);
    }
    void Physic::update(sf::Time delta){
        float d = delta.asSeconds();
        if (m_gravity) m_velocity += s_gravity * d;
        m_velocity += m_acceleration * d;
        Component::m_ptrEntity->move(m_velocity * s_meter * d);
    }
    std::string Physic::to_string() const{
        const char* format = 
        "physic gravity = {}; velocity = {{{}, {}}}; acceleration = {{{}, {}}}; mass = {}";
        return fmt::format(format, 
                           m_gravity ? "on":"off",
                           m_velocity.x,
                           m_velocity.y,
                           m_acceleration.x,
                           m_acceleration.y,
                           m_mass
                          );
    }
    Component* parsePhysic(const std::string& line){
        auto ret = new Physic();
        auto dict = parseMap(line);
        for(auto& kv : dict){
            if(kv.first == "gravity"){
                if(kv.second == "on"){
                    ret->m_gravity = true;
                }else if(kv.second == "off"){
                    ret->m_gravity = false;
                }else{
                    LOG_F(WARNING, "Unrecognized value for gravity in Physic: {}", kv.second);
                }
            }else if(kv.first == "velocity"){
                ret->m_velocity = parseVector2<float>(kv.second);
            }else if(kv.first == "acceleration"){
                ret->m_acceleration = parseVector2<float>(kv.second);
            }else if(kv.first == "mass"){
                ret->m_mass = std::stof(kv.second);
            }else{
                LOG_F(WARNING, "Unrecognized key for Physic: {}", kv.first);
            }
        }
        return ret;
    }
}
