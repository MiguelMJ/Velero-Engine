#ifndef __COLLIDERCOMPONENT_HPP__
#define __COLLIDERCOMPONENT_HPP__

#include "SFML/Graphics/VertexArray.hpp"

#include "VelEng/Component.hpp"
#include "VelEng/Entity.hpp"
#include "VelEng/Renderable.hpp"
#include "VelEng/geometry.hpp"

namespace ge{
    class Collider : public Component, public Renderable{
    public:
        bool m_solid = false;
        int m_priority = 0;
        Polygon m_basePolygon;
        Polygon m_transformedPolygon;
        Collider();
        void handle(const Event* event, std::type_index type, size_t channel) override;
        Component* copy() const override;
        void onActivate() override;
        void onDeactivate() override;
        std::string to_string() const override;
        sf::FloatRect getGlobalBounds() const override;
    private:
        void draw(sf::RenderTarget& t, sf::RenderStates s) const override;
    };
    Component* parseCollider(const std::string& in);
}

#endif
