#include <iostream>
#include <sstream>
#include <map>

#include "SFML/Graphics.hpp"

#include "AssetSystem.hpp"
#include "Random.hpp"

#define AS AssetSystem

using namespace ge;
using namespace ge::Random;
using namespace std;

map <AssetType, string> assetTypeName = {
    {Texture, "Texture"},
    {Sound, "Sound"},
    {Font, "Font"},
    {Font, "Tileset"},
    {Tilemap, "Tilemap"},
    {Animation, "Animation"},
    {EventScript, "EventScript"},
    {Prototype, "Prototype"},
    {Scene, "Scene"},
    {Unrecognized, "Unrecognized"}
};
map <AssetState, string> assetStateName = {
    {Unloaded, "Unloaded"},
    {Loaded, "Loaded"},
    {Error, "Error"}
};

void print(const AssetInfo& ai){
    cout 
         << "(" << assetStateName[ai.state] << ") "
         << assetTypeName[ai.type] 
         << ": "
         << ai.name
         << " [" << ai.timesAccessed << "]"
         << endl;
}

int main(int argc, char** argv){
    
    AS::addPath("/home/miguel_mj/Programs/ASSETS");
    
    AS::setDefaultFont("FONTS/Hasenchartbreaker.ttf");
    AS::loadRecursively("SOUNDS");
    std::string gp = "TEXTURES/Golem/attack/hit_1.png";
    AS::load(gp);
    
    const auto& allassetinfo = AS::getInfo();
    
    cout << "ASSET INFO" << endl;
    cout << "==========" << endl;
    for(auto& ai: allassetinfo){
        print(ai);
    }
    
    sf::Text t("Hola assets", *AS::getFont(), 50);
    t.move(50,60);
    sf::RenderWindow window;
    window.create(sf::VideoMode(500,500), "assets01");
    
    sf::Sound sound;
    sf::Sprite golem(*AS::getTexture(gp));
    
    while(window.isOpen()){
        sf::Event e;
        while(window.pollEvent(e)){
            switch(e.type){
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                stringstream ss;
                ss << "SOUNDS/teclas/tap" << rndInt(1,6) << ".wav";
                sound.setBuffer(*AS::getSound(ss.str()));
                sound.play();
                
                if(e.key.code == sf::Keyboard::I){
                    const auto& allassetinfo = AS::getInfo();
                    cout << "ASSET INFO" << endl;
                    cout << "==========" << endl;
                    for(auto& ai: allassetinfo){
                        print(ai);
                    }
                }
                
                break;
            }
        }
        
        window.clear();
        window.draw(golem);
        window.draw(t);
        window.display();
    }
    return 0;
}
