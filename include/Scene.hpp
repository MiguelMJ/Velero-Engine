#ifndef __GE_SCENE_HPP__
#define __GE_SCENE_HPP__

#include <map>
#include <memory>
#include <fstream>

#include "EntityComponent.hpp"
#include "Prototype.hpp"

namespace ge{
    typedef std::pair<unsigned long, std::unique_ptr<Entity> > identitypair;
    class Scene{
    private:
        bool m_active=false;
        std::map<unsigned long, std::unique_ptr<Entity>> m_entities;
        std::map<std::string, unsigned long> m_entityIndex;
        unsigned long m_nextID;
    public:
        Entity* addEntity(Prototype* prototype, bool active=true, const std::string& name="");
        Entity* addEntity(bool active=true, const std::string& name="");
        void removeEntity(unsigned long id);
        bool loadFromFile(const std::string& file);
        void setActive(bool active);
    };
}
// At the end because of dependencies
#include "AssetSystem.hpp"

#endif
