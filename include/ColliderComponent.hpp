#ifndef __COLLIDERCOMPONENT_HPP__
#define __COLLIDERCOMPONENT_HPP__

#include <istream>

#include "SFML/Graphics/VertexArray.hpp"

#include "EntityComponent.hpp"
#include "Renderable.hpp"
#include "geometry.hpp"

namespace ge{
    class Collider : public Component, public Renderable{
    public:
        int m_priority = 0;
        Polygon m_basePolygon;
        Polygon m_transformedPolygon;
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
