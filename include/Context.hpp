#ifndef __GE_CONTEXT_HPP__
#define __GE_CONTEXT_HPP__

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "RenderSystem.hpp"
#include "AssetSystem.hpp"
#include "EventSystem.hpp"
#include "Scene.hpp"

#define assetSystem AssetSystem
#define eventSystem EventSystem

#define M_RS ge::renderSystem
#define M_AS ge::AssetSystem
#define M_ES ge::EventSystem

namespace ge{
    extern RenderSystem renderSystem;
    
    void setCurrentScene(Scene* scene);
    Scene* getCurrentScene();
    void launch();
}

#endif
