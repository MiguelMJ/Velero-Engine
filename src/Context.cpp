#include "Context.hpp"

namespace ge{
    
    Scene g_defaultScene;
    Scene *g_ptrCurrentScene = &g_defaultScene;
    Scene *g_ptrNextScene = g_ptrCurrentScene;
    sf::Clock clock;
    
    sf::RenderWindow window;
    RenderSystem renderSystem;
    TimeSystem logicTimeSystem;
    TimeSystem visualTimeSystem;
    CollisionSystem collisionSystem;
    
    void setNextScene(Scene* scene){
        g_ptrNextScene = scene;
    }
    Scene* getCurrentScene(){
        return g_ptrCurrentScene;
    }
    void launch(){
        while(window.isOpen()){
            sf::Time delta(clock.restart());
            sf::Event event;
            while(window.pollEvent(event)){
                M_IS::dispatch(event);
                if(event.type == sf::Event::Closed){
                    window.close();
                }
            }
            
            if(g_ptrNextScene != g_ptrCurrentScene){
                g_ptrCurrentScene->setActive(false);
                g_ptrCurrentScene = g_ptrNextScene;
                g_ptrCurrentScene->setActive(true);
            }
            
            
            logicTimeSystem.update(delta);
            visualTimeSystem.update(delta);
            
            // collisionSystem.solveCollisions();
            
            window.clear();
            renderSystem.draw(window);
            window.display();
        }
        Entity::s_removeComponents=false;
    }
}
