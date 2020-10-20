/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the Scene class.
 */
#ifndef __GE_SCENE_HPP__
#define __GE_SCENE_HPP__

#include <map>
#include <memory>
#include <fstream>

#include "VelEng/Entity.hpp"
#include "VelEng/Prototype.hpp"

namespace ge{
    typedef std::pair<unsigned long, std::unique_ptr<Entity> > identitypair;
    typedef std::map<unsigned long, std::unique_ptr<Entity> >::iterator iterator;
    typedef std::map<unsigned long, std::unique_ptr<Entity> >::const_iterator const_iterator;
    class Scene{
    private:
        bool m_active=false;
        std::map<unsigned long, std::unique_ptr<Entity>> m_entities;
        std::map<std::string, unsigned long> m_entityIndex;
        unsigned long m_nextID=0;
    public:
        /**
         * @brief Add an entity generated on a prototype, set its active 
         * flag and name.
         * @param prototype Pointer to the prototype to generate the 
         * entity.
         * @param active Flag to activate the entity. If the scene is 
         * inactive, this one has no effect.
         * @param name Name of the enerated entity.
         * @returns A pointer to the generated entity.
         * @see setActive removeEntity
         */
        Entity* addEntity(Prototype* prototype, bool active=true, const std::string& name="");
        
        /**
         * @brief Add an empty, set its active flag and name.
         * @param active Flag to activate the entity. If the scene is 
         * inactive, this one has no effect.
         * @param name Name of the enerated entity.
         * @returns A pointer to the generated entity.
         * @see setActive removeEntity
         */
        Entity* addEntity(bool active=true, const std::string& name="");
        
        /**
         * @brief Remove an entity, given its numeric identifier.
         * @param id Numeric identifier of the entity.
         * @see addEntity
         */
        void removeEntity(unsigned long id);
        /**
         * @brief Loads the scene from a file.
         * @
         * @param file Path to the file.
         * @returns True if the loading was succesful.
         */
        bool loadFromFile(const std::string& file);
        
        /**
         * @brief Set the _active_ flag of the scene.
         * @details If If the new value of the active flag is the same as the old, 
         * it has no effect. Otherwise, the active flag is set the same for all 
         * the entities of the scene. By default it is inactive.
         */
        void setActive(bool active);
        
        /**
         * @brief Get a pointer to an entity, given its name.
         * @returns A pointer to the entity, if it exists; null otherwise.
         */
        Entity* getEntity(const std::string& name);
        
        /**
         * @brief Get a pointer to an entity, given its numeric identifier.
         * @returns A pointer to the entity, if it exists; null otherwise.
         */
        Entity* getEntity(unsigned long id);
        
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        size_t entityCount() const;
        size_t namedEntitiesCount() const;
    };
}

#endif
