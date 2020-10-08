#include "Context.hpp"

#include "SpriteComponent.hpp"
#include "PhysicComponent.hpp"
#include "ColliderComponent.hpp"
#include "PlatformerControllerComponent.hpp"

#include "fmt/core.h"
using namespace ge;

int main(){
    
    ComponentParser::registerComponent("sprite", parseSprite);
    ComponentParser::registerComponent("physic", parsePhysic);
    ComponentParser::registerComponent("collider", parseCollider);
    ComponentParser::registerComponent("platformer", parsePlatformerController);
    
    M_RSD.pushLayer("debug");
    M_RS.pushLayer("background");
    M_RS.pushLayer("behind");
    M_RS.pushLayer("middle");
    M_RS.pushLayer("front");
    M_RS.pushLayer("foremost");
    
    M_AS::addPath("assets");
    M_AS::addPath("/home/miguel_mj/Programs/ASSETS/TEXTURES/");
    M_AS::setDefaultFont("/home/miguel_mj/Programs/ASSETS/FONTS/Pixel Gosub.ttf");
    M_AS::loadRecursively("assets");
    
    auto ll = M_RS.pushLightingLayer("light");
    ll->setFogOpacity(0);
    
    ge::window.create(sf::VideoMode(500,500), "app");
    ge::window.setFramerateLimit(60);
    
    // user code here
    auto s = M_AS::getScene("level0.scene");
    ge::setNextScene(s);
    
    launch();
    return 0;
}
