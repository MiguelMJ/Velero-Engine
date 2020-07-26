#ifndef __GE_SCENE_HPP__
#define __GE_SCENE_HPP__

#include <map>

#include "EntityComponent.hpp"

namespace ge{
    class Scene{
    private:
        std::map<unsigned long, std::unique_ptr<Entity>> m_entities;
        std::map<std::string, unsigned long> m_entityIndex;
        unsigned long nextID;
    public:
        
        
    };
}

#endif
