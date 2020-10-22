#include "VelEng/PlatformerControllerComponent.hpp"

#include "VelEng/Entity.hpp"
#include "VelEng/Context.hpp"
#include "VelEng/PhysicComponent.hpp"
#include "VelEng/CollisionEvent.hpp"
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
    void PlatformerController::draw(sf::RenderTarget& t, sf::RenderStates s)const{
        s.transform *= getEntityPtr()->getTransform();
        sf::Text txt(isInAir?"AIR":"FLOOR", *M_AS::getFont( ));
        t.draw(txt, s);
    }
    void PlatformerController::onActivate(){
        M_LTS.addUpdatable(this);
        M_RSD.getLayer("debug")->addRenderable(this);
    }
    void PlatformerController::onDeactivate(){
        M_LTS.addUpdatable(this);
        M_RSD.getLayer("debug")->removeRenderable(this);
    }
    void PlatformerController::update(sf::Time){/*
        float moveSpeed = 10;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            Effect<Physic, float> ef = {
                -moveSpeed*3,
                setSpeedY
            };
            getEntityPtr()->apply(ef);
            isInAir = true;
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            Effect<Physic, float> ef = {
                0,
                setSpeedY
            };
            getEntityPtr()->apply(ef);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            Effect<Physic, float> ef = {
                moveSpeed,
                setSpeedX
            };
            getEntityPtr()->apply(ef);
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            Effect<Physic, float> ef = {
                -moveSpeed,
                setSpeedX
            };
            getEntityPtr()->apply(ef);
        }else{
            Effect<Physic, float> ef = {
                0,
                setSpeedX
            };
            getEntityPtr()->apply(ef);
        }
    */}
    std::string PlatformerController::to_string() const{
        return "";
    }
    Component* PlatformerController::copy() const{
        return new PlatformerController();
    }
    Component* parsePlatformerController(const std::string&){
        PlatformerController* pc = new PlatformerController;
        return pc;
    }
}
