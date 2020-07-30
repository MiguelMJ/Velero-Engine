#include "Context.hpp"

namespace ge{
    Scene g_defaultScene;
    Scene *g_ptrCurrentScene = &g_defaultScene;
    Scene *g_ptrNextScene = g_ptrCurrentScene;
    sf::RenderWindow window;
    
    RenderSystem renderSystem;
    
    void setCurrentScene(Scene* scene){
        g_ptrNextScene = scene;
    }
    Scene* getCurrentScene(){
        return g_ptrCurrentScene;
    }
    void launch(){
        while(window.isOpen()){
            sf::Event event;
            while(window.pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window.close();
                }
            }
            
            if(g_ptrNextScene != g_ptrCurrentScene){
                g_ptrCurrentScene->setActive(false);
                g_ptrCurrentScene = g_ptrNextScene;
                g_ptrCurrentScene->setActive(true);
            }
            
            window.clear();
            M_RS.draw(window);
            window.display();
        }
        Entity::s_removeComponents=false;
    }
}