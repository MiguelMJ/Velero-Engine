#include "Context.hpp"

#include "SpriteComponent.hpp"

#include "fmt/core.h"
using namespace ge;

int main(){
    
    ComponentParser::registerComponent("sprite", parseSprite);
    
    M_RS.pushLayer("background");
    M_RS.pushLayer("behind");
    M_RS.pushLayer("middle");
    M_RS.pushLayer("front");
    M_RS.pushLayer("foremost");
    
    M_AS::addPath("assets");
    M_AS::addPath("/home/miguel_mj/Programs/ASSETS/TEXTURES/");
    M_AS::loadRecursively("assets");
    
    auto ll = M_RS.pushLightingLayer("light");
    ll->setFogOpacity(0);
    
    ge::window.create(sf::VideoMode(500,500), "app");
    ge::window.setFramerateLimit(60);
    
    // user code here
    auto s = M_AS::getScene("level0.scene");
    ge::setCurrentScene(s);
    
    LOG_F(INFO, "{}({}) entities in scene", s->entityCount(), s->namedEntitiesCount());
    for(auto it = s->begin(); it != s->end(); it++){
        LOG_F(INFO, "Entity [{}]: {} \"{}\"", it->first, it->second->getId(), it->second->getName());
    }
    
    LOG_F(INFO, "Removing entity 1");
    s -> removeEntity(1);
    launch();
    return 0;
}
