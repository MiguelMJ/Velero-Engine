#ifndef __GE_CONTEXT_HPP__
#define __GE_CONTEXT_HPP__

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "RenderSystem.hpp"
#include "AssetSystem.hpp"
#include "TimeSystem.hpp"
#include "EventSystem.hpp"
#include "InputSystem.hpp"
#include "CollisionSystem.hpp"
#include "Scene.hpp"

#define assetSystem AssetSystem
#define eventSystem EventSystem

#define M_RS ge::renderSystem
#define M_RSD ge::renderSystemD
#define M_AS ge::AssetSystem
#define M_ES ge::EventSystem
#define M_IS ge::InputSystem
#define M_LTS ge::logicTimeSystem
#define M_VTS ge::visualTimeSystem
#define M_CS ge::collisionSystem

namespace ge{
    void setNextScene(Scene* scene);
    Scene* getCurrentScene();
    void launch();
    
    extern RenderSystem renderSystem;
    extern RenderSystem renderSystemD;
    extern TimeSystem logicTimeSystem;
    extern TimeSystem visualTimeSystem;
    extern CollisionSystem collisionSystem;
    extern sf::RenderWindow window;
}

#endif
