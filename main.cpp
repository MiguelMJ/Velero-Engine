#include "Context.hpp"

#include "SpriteComponent.hpp"
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
    auto scene = M_AS::getScene("level0.scene");
    scene->setActive(true);
    
    launch();
    return 0;
}
