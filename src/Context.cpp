#include "Context.hpp"

namespace ge{
    Scene g_defaultScene;
    Scene *g_ptrCurrentScene = &g_defaultScene;
    Scene *g_ptrNextScene = g_ptrCurrentScene;
    
    RenderSystem renderSystem;
    
    void setCurrentScene(Scene* scene){
        g_ptrNextScene = scene;
    }
    Scene* getCurrentScene(){
        return g_ptrCurrentScene;
    }
    void launch(){
        
    }
}
