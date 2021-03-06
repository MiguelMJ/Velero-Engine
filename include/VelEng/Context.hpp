#ifndef __GE_CONTEXT_HPP__
#define __GE_CONTEXT_HPP__

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "box2d/b2_world.h"

#include "VelEng/RenderSystem.hpp"
#include "VelEng/AssetSystem.hpp"
#include "VelEng/TimeSystem.hpp"
#include "VelEng/EventSystem.hpp"
#include "VelEng/InputSystem.hpp"
#include "VelEng/CollisionSystem.hpp"
#include "VelEng/Scene.hpp"

#define PIXELS_METER 50.f

#define assetSystem AssetSystem
#define eventSystem EventSystem

#define M_RS ven::renderSystem
#define M_RSD ven::renderSystemD
#define M_AS ven::AssetSystem
#define M_ES ven::EventSystem
#define M_IS ven::InputSystem
#define M_LTS ven::logicTimeSystem
#define M_VTS ven::visualTimeSystem
#define M_PS ven::physicsSystem

namespace ven{
    void setNextScene(Scene* scene);
    Scene* getCurrentScene();
    void launch();
    
    extern RenderSystem renderSystem;
    extern RenderSystem renderSystemD;
    extern TimeSystem logicTimeSystem;
    extern TimeSystem visualTimeSystem;
    extern sf::RenderWindow window;
    extern b2World physicsSystem;
    
    void centerWindow();
    
}

#endif
