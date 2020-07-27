#include "Context.hpp"

#include "SpriteComponent.hpp"
using namespace ge;

int main(int argc, char** argv){
    
    ComponentParser::registerComponent("sprite", parseSprite);
    
    M_AS::addPath("assets");
    M_AS::addPath("/home/miguel_mj/Programs/ASSETS/TEXTURES/");
    M_AS::loadRecursively("assets");
    
    M_RS.pushLayer("background");
    M_RS.pushLayer("behind");
    M_RS.pushLayer("middle");
    M_RS.pushLayer("front");
    M_RS.pushLayer("foremost");
    auto ll = M_RS.pushLightingLayer("light");
    ll->setFogOpacity(0);
    
    ge::window.create(sf::VideoMode(500,500), "app");
    ge::window.setFramerateLimit(60);
    auto e = M_AS::getPrototype("test.prototype")->generate(true);
    
    launch();
    return 0;
}
