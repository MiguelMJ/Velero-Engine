#include "VelEng/PlatformerControllerComponent.hpp"

#include "VelEng/Entity.hpp"
#include "VelEng/Context.hpp"
#include "VelEng/PhysicComponent.hpp"
#include "VelEng/CollisionEvent.hpp"
#include "VelEng/RigidBody.hpp"
#include "VelEng/AssetSystem.hpp"

#include <map>

namespace ven{
    
    PlatformerController::PlatformerController(){
        // Component::m_eventsHandled.insert(typeid(CollisionEvent));
    }
    void PlatformerController::handle(const Event* , std::type_index, size_t){
        // isInAir = isInAir && ((CollisionEvent*)ev)->solution.y > 0;
    }
    sf::FloatRect PlatformerController::getGlobalBounds()const{
        return getEntityPtr()->getTransform().transformRect(sf::FloatRect());
    }
    void PlatformerController::draw(sf::RenderTarget& , sf::RenderStates )const{
        /*s.transform *= getEntityPtr()->getTransform();
        sf::Text txt(isInAir?"AIR":"FLOOR", *M_AS::getFont(), 5);
        t.draw(txt, s);*/
    }
    void PlatformerController::onActivate(){
        M_LTS.addUpdatable(this);
        M_RSD.getLayer("debug")->addRenderable(this);
    }
    void PlatformerController::onDeactivate(){
        M_LTS.addUpdatable(this);
        M_RSD.getLayer("debug")->removeRenderable(this);
    }
    void PlatformerController::update(sf::Time){
        float moveSpeed = 1;
        float jumpForce = 10;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            Effect<RigidBody, float> ef = {
                jumpForce,
                [](RigidBody& b, const float& n){
                    b.m_body->ApplyForceToCenter({0,n}, true);
                }
            };
            getEntityPtr()->apply(ef);
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            Effect<RigidBody, float> ef = {
                moveSpeed,
                [](RigidBody& b, const float& n){
                    auto v = b.m_body->GetLinearVelocity();
                    v.x+=n;
                    b.m_body->SetLinearVelocity(v);
                }
            };
            getEntityPtr()->apply(ef);
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            Effect<RigidBody, float> ef = {
                moveSpeed,
                [](RigidBody& b, const float& n){
                    auto v = b.m_body->GetLinearVelocity();
                    v.x-=n;
                    b.m_body->SetLinearVelocity(v);
                }
            };
            getEntityPtr()->apply(ef);
        }else{
            
        }
    }
    std::string PlatformerController::to_string() const{
        return "";
    }
    Component* PlatformerController::copy() const{
        return new PlatformerController();
    }
    Component* parsePlatformerController(const JSON&){
        PlatformerController* pc = new PlatformerController;
        return pc;
    }
}
