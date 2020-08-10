#include "PhysicComponent.hpp"

#include "Context.hpp"
#include "stringmanip.hpp"

namespace ge{
    float Physic::s_meter(64);
    sf::Vector2f Physic::s_gravity(0,9.1);
    void Physic::onActivate(){
        M_LTS.addUpdatable(this);
    }
    void Physic::onDeactivate(){
        M_LTS.removeUpdatable(this);
    }
    void Physic::handle(const Event*, std::type_index, size_t){
        
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
    Component* parsePhysic(std::istream& in){
        auto ret = new Physic();
        std::string line;
        getline(in, line);
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
