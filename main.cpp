#include "AssetSystem.hpp"
#include "RenderSystem.hpp"
#include "EventSystem.hpp"

#include "SFML/Graphics.hpp"

#define AS AssetSystem
#define RS RenderSystem
#define ES EventSystem

using namespace ge;

typedef struct {
    typedef struct {
        bool dynamic;
        bool light;
        std::string name;
    } LayerConfiguration;
    
    std::string title;
    bool fullscreen;
    int width, height;
    std::vector<LayerConfiguration> renderConfiguration;
    std::vector<std::string> assetPaths;
    bool autoload;
    bool autofix;
    std::vector<std::string> logFiles;
    int loglevel;
} AppConfiguration;

int main(int argc, char** argv){
    
    
    
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
        
        RS::draw(window);
        
        window.display();
    }
    return 0;
}
