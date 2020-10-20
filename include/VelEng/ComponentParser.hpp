#ifndef __COMPONENTPARSER_HPP__
#define __COMPONENTPARSER_HPP__

#include <istream>
#include <map>
#include <string>
#include "VelEng/log.hpp"
#include "VelEng/Component.hpp"

namespace ven{
    namespace ComponentParser{
        typedef Component* (*cparsefunc) (const std::string& in);
        Component* parse(std::istream& in);
        void registerComponent(const std::string& name, cparsefunc func);
    }
}

#endif
