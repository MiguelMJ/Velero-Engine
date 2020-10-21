#ifndef __COMPONENTPARSER_HPP__
#define __COMPONENTPARSER_HPP__

#include <istream>
#include <map>
#include <string>
#include "VelEng/log.hpp"
#include "VelEng/Component.hpp"
#include "VelEng/Serialize/Parser.hpp"

namespace ven{
    namespace ComponentParser{
        typedef Component* (*cparsefunc) (const JSON& json);
        Component* parse(const JSON& json);
        void registerComponent(const std::string& name, cparsefunc func);
    }
}

#endif
