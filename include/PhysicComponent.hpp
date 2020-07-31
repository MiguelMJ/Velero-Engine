#ifndef __PHYSICCOMPONENT_HPP__
#define __PHYSICCOMPONENT_HPP__

#include <istream>

#include "SFML/System/Vector2.hpp"
#include "SFML/System/Time.hpp"

#include "EntityComponent.hpp"
#include "Updatable.hpp"

namespace ge{
    class Physic: public Updatable, public Component{
    public:
        static sf::Vector2f s_gravity;
        static float s_meter;
        sf::Vector2f m_velocity;
        sf::Vector2f m_acceleration;
        float m_mass=1;
        bool m_gravity=true;
        void onActivate() override;
        void onDeactivate() override;
        void handle(const Event* event, std::type_index type, size_t channel) override;
        Component* copy() const override;
        void update(sf::Time delta) override;
    };
    Component* parsePhysic(std::istream& in);
}

#endif