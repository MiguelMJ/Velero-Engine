#ifndef __COLLIDERCOMPONENT_HPP__
#define __COLLIDERCOMPONENT_HPP__

#include <istream>

#include "geometry.hpp"
#include "EntityComponent.hpp"

namespace ge{
    class Collider : public Component{
    public:
        int m_priority = 0;
        Polygon m_basePolygon;
        Polygon m_transformedPolygon;
        Component* copy() const override;
        void onActivate() override;
        void onDeactivate() override;
        std::string to_string() const override;
    };
    Component* parseCollider(std::istream& in);
}

#endif
