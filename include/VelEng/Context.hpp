#ifndef __GE_CONTEXT_HPP__
#define __GE_CONTEXT_HPP__

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "VelEng/RenderSystem.hpp"
#include "VelEng/AssetSystem.hpp"
#include "VelEng/TimeSystem.hpp"
#include "VelEng/EventSystem.hpp"
#include "VelEng/InputSystem.hpp"
#include "VelEng/CollisionSystem.hpp"
#include "VelEng/Scene.hpp"

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
