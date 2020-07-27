#ifndef __PROTOTYPE_HPP__
#define __PROTOTYPE_HPP__

#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "EntityComponent.hpp"
#include "ComponentParser.hpp"
#include "stringmanip.hpp"

namespace ge{
    class Prototype final{
    private:
        int m_generated;
    public:
        std::string m_name;
        std::string m_nameOfBase;
        Prototype* m_base=nullptr;
        std::vector<std::unique_ptr<Component> > m_components;
        Entity* generate(bool active=true);
        bool loadFromFile(const std::string& file);
    };
}

#endif
