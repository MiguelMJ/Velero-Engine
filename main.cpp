#include "SFML/System.hpp"

#include "VelEng/Context.hpp"

#include "VelEng/SpriteComponent.hpp"
#include "VelEng/PhysicComponent.hpp"
#include "VelEng/ColliderComponent.hpp"
#include "VelEng/PlatformerControllerComponent.hpp"

#include "VelEng/GameConfig.hpp"
#include "VelEng/Serialize/GameConfig.hpp"
#include "VelEng/log.hpp"

#include "fmt/core.h"
using namespace ven;

#include "data/icon32.c"
#include "data/splash.c"

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
    if(!config.stderr){
        loguru::g_stderr_verbosity = -5;
    }
    if(config.infolog.size() > 0){
        loguru::add_file(config.infolog.c_str(), loguru::Truncate, loguru::Verbosity_INFO);
    }
    if(config.errlog.size() > 0){
        loguru::add_file(config.errlog.c_str(), loguru::Truncate, loguru::Verbosity_WARNING);
    }
    ComponentParser::registerComponent("sprite", parseSprite);
    /*ComponentParser::registerComponent("physic", parsePhysic);
    ComponentParser::registerComponent("collider", parseCollider);
    ComponentParser::registerComponent("platformer", parsePlatformerController);*/
    
    M_AS::setDefaultFont("/home/miguel_mj/Programs/ASSETS/FONTS/Pixel Gosub.ttf");
    
    if(config.splashscreen){
        float ratio = (float)splash.height/splash.width;
        float ww = 800;
        window.create(sf::VideoMode(ww,ww*ratio), "", sf::Style::None);
        sf::Image splashIm;
        splashIm.create(splash.width, splash.height, splash.pixel_data);
        sf::Texture t;
        if(t.loadFromImage(splashIm)){
            t.setSmooth(true);
            sf::Sprite im(t);
            auto s = t.getSize();
            auto ws = window.getSize();
            centerWindow();
            im.scale((float)ws.x/s.x, (float)ws.y/s.y);
            sf::Color col = sf::Color::White;
            col.a=0;
            sf::Clock c;
            float elapsed = 0;
            float total = 3.3;
            do{
                if(elapsed < total/5){
                    col.a = 255*(5*elapsed/total);
                }else if(elapsed > total*4/5){
                    col.a = 255*((total - elapsed)/(total*1/5));
                }
                im.setColor(col);
                window.clear();
                window.draw(im);
                window.display();
                elapsed = c.getElapsedTime().asSeconds();
            }while(elapsed < total);
            // window.close();
        }else{
            LOG_F(WARNING,"No splash screen");
        }
    }
    
    if(config.fullscreen){
        ven::window.create(sf::VideoMode(), config.title, sf::Style::Fullscreen);
    }else{
        ven::window.create(sf::VideoMode(config.width,config.height), config.title);
    }
    ven::window.setIcon(icon.width, icon.height, icon.pixel_data);
    
    ven::window.setFramerateLimit(60);
    
    centerWindow();
    
    auto s = M_AS::getScene(config.entry);
    ven::setNextScene(s);
    
    launch();
    return 0;
}

