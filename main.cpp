#include "SFML/System.hpp"

#include "VelEng/Context.hpp"

#include "VelEng/SpriteComponent.hpp"
#include "VelEng/PhysicComponent.hpp"
#include "VelEng/ColliderComponent.hpp"
#include "VelEng/PlatformerControllerComponent.hpp"

#include "VelEng/GameConfig.hpp"
#include "VelEng/Serialize/GameConfig.hpp"

#include "fmt/core.h"
using namespace ven;

int main(){
    
    GameConfig config = parsef<GameConfig>("game.config");
    
    for(auto& layer : config.layers){
        if(layer[0] == 'L'){
            auto ll = M_RS.pushLightingLayer(layer);
            ll->setFogOpacity(0);
        }else{
            M_RS.pushLayer(layer);
        }
    }
    if(config.debugrender){
        M_RSD.pushLayer("debug");
    }
    
    for(auto& p : config.path){
        M_AS::addPath(p);
    }
    
    /*ComponentParser::registerComponent("sprite", parseSprite);
    ComponentParser::registerComponent("physic", parsePhysic);
    ComponentParser::registerComponent("collider", parseCollider);
    ComponentParser::registerComponent("platformer", parsePlatformerController);*/
    
    M_AS::setDefaultFont("/home/miguel_mj/Programs/ASSETS/FONTS/Pixel Gosub.ttf");
    
    if(config.splashscreen){
        window.create(sf::VideoMode(400,400*1285/1080), "", sf::Style::None);
        sf::Texture t;
        if(t.loadFromFile("doc/logo_compact.jpg")){
            t.setSmooth(true);
            sf::Sprite im(t);
            auto s = t.getSize();
            auto ws = window.getSize();
            centerWindow();
            im.scale((float)ws.x/s.x, (float)ws.y/s.y);
            window.draw(im);
            window.display();
            sf::sleep(sf::seconds(2));
            window.close();
        }else{
            LOG_F(WARNING,"No splash screen");
        }
    }
    
    if(config.fullscreen){
        ven::window.create(sf::VideoMode(), config.title, sf::Style::Fullscreen);
    }else{
        ven::window.create(sf::VideoMode(config.width,config.height), config.title);
    }
    
    ven::window.setFramerateLimit(60);
    
    centerWindow();
    
    auto s = M_AS::getScene(config.entry);
    ven::setNextScene(s);
    
    launch();
    return 0;
}
