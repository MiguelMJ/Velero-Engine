#ifndef __PHYSICCOMPONENT_HPP__
#define __PHYSICCOMPONENT_HPP__

#include <istream>

#include "SFML/System/Vector2.hpp"
#include "SFML/System/Time.hpp"

#include "VelEng/Component.hpp"
#include "VelEng/Updatable.hpp"

namespace ven{
    class Physic: public Updatable, public Component{
    public:
        static sf::Vector2f s_gravity;
        static float s_meter;
        sf::Vector2f m_velocity;
        sf::Vector2f m_acceleration;
        float m_mass=1;
        bool m_gravity=true;
        Physic();
        void onActivate() override;
        void onDeactivate() override;
        void handle(const Event* event, std::type_index type, size_t channel) override;
        std::string to_string() const override;
        Component* copy() const override;
        void update(sf::Time delta) override;
    };
    Component* parsePhysic(const std::string& in);
}

#endif
