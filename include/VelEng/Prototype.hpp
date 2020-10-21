/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the Prototype class.
 */
#ifndef __PROTOTYPE_HPP__
#define __PROTOTYPE_HPP__

#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "VelEng/Entity.hpp"
#include "VelEng/ComponentParser.hpp"
#include "VelEng/stringmanip.hpp"

namespace ven{
    /**
     * @brief Blueprint of an [entity](@ref Entity).
     */
    class Prototype final{
    private:
        int m_generated=0;
    public:
        /**
         * @brief Default name given to the entities generated by this 
         * prototype.
         * @details Two different prototypes can have the same name, but the 
         * generated entities can't be referenced by it safely. The 
         * generated name will have the ordinal of generation appended.
         */
        std::string m_name;
        
        /**
         * @brief Path of the base prototype.
         * @see m_base
         */
        std::string m_nameOfBase;
        
        /**
         * @brief Base prototype.
         * @details If it is not null, every component of the current 
         * prototype will be added to the entity generated by the base 
         * protoype, instead of using an empty one.
         */
        Prototype* m_base=nullptr;
        
        /**
         * @brief Components that define the entities to generate.
         */
        std::vector<std::unique_ptr<Component> > m_components;
        
        /**
         * @brief Generate an entity from this prototype.
         * @param active True to activate the generated entity.
         * @param name If the string is not empty, the generated entity 
         * will have this name.
         * @returns Pointer to the generated Entity.
         */
        Entity* generate(unsigned long id=0, std::string name = "", bool active=true);
        
        /**
         * @brief Load the prototype from a stream.
         * @param in Input value.
         * @returns True if the loading was succesful.
         */
        bool loadFromJSON(const JSON& json);
        
        /**
         * @brief Load the prototype from a rapidjson::value.
         * @param in Input string.
         * @returns True if the loading was succesful.
         */
        bool loadFromString(const std::string& str);
        
        /**
         * @brief Load the prototype from a file.
         * @param file Path to the file.
         * @returns True if the loading was succesful.
         */
        bool loadFromFile(const std::string& file);
        
        ~Prototype();
    };
}

#endif
