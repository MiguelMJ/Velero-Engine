#ifndef __COMPONENTPARSER_HPP__
#define __COMPONENTPARSER_HPP__

#include <istream>
#include <map>
#include <string>
#include "log.hpp"
#include "EntityComponent.hpp"

namespace ge{
    namespace ComponentParser{
        typedef Component* (*cparsefunc) (std::istream& in);
        Component* parse(std::istream& in);
        void registerComponent(const std::string& name, cparsefunc func);
    }
}

#endif