#ifndef __GE_SCENE_HPP__
#define __GE_SCENE_HPP__

#include <map>

#include "EntityComponent.hpp"

namespace ge{
    typedef std::pair<unsigned long, std::unique_ptr<Entity> > identitypair;
    class Scene{
    private:
        std::map<unsigned long, std::unique_ptr<Entity>> m_entities;
        std::map<std::string, unsigned long> m_entityIndex;
        unsigned long nextID;
    public:
        // Entity* addEntity(const Prototype);
        Entity* addEntity(bool active = true, const std::string& name="");
        void removeEntity(unsigned long id);
    };
}

#endif
