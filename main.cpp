#include "AssetSystem.hpp"
#include "RenderSystem.hpp"
#include "EventSystem.hpp"

#include "SFML/Graphics.hpp"

#define AS AssetSystem
#define RS RenderSystem
#define ES EventSystem

using namespace ge;

int main(int argc, char** argv){
    
    RS rs;
    
    sf::RenderWindow window;
    window.create(sf::VideoMode(500,500), "main");
    
    while(window.isOpen()){
        sf::Event e;
        while(window.pollEvent(e)){
            switch(e.type){
            case sf::Event::Closed:
                window.close();
                break;
            }
        }
        
        ES::dispatch();
        
        window.clear();
        
        rs.draw(window);
        
        window.display();
    }
    return 0;
}
