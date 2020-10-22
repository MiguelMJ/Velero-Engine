#ifndef __VELERO_RIGIDBODY_HPP_
#define __VELERO_RIGIDBODY_HPP__

#include <vector>

#include "VelEng/Updatable.hpp"
#include "VelEng/Renderable.hpp"
#include "VelEng/Component.hpp"

#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"


namespace ven{
    class RigidBody: public Component, public Renderable, public Updatable{
    private:
        b2BodyDef m_bodyDef;
        std::vector<b2FixtureDef> m_fixtureDefs;
        friend Component* parseRigidBody(const std::string&);   
    public:
        RigidBody();
        b2Body* m_body;
        void draw(sf::RenderTarget&, sf::RenderStates) const override;
        sf::FloatRect getGlobalBounds() const override;
        void update(sf::Time) override;
        void onAdd() override;
        void onRemove() override;
        void onActivate() override;
        void onDeactivate() override;
        std::string to_string() const override;
        Component* copy() const override;
    };
    Component* parseRigidBody (const std::string&);
}

#endif
