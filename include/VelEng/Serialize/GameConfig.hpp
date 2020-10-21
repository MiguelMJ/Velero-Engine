#ifndef __VELERO_GAMECONFIG_HPP__
#define __VELERO_GAMECONFIG_HPP__

#include "VelEng/GameConfig.hpp"
#include "VelEng/Serialize/Parser.hpp"

namespace ven{
    template<>
    GameConfig parseDOM<GameConfig>(const JSON& json){
        GameConfig config;
        config.title = DOMget<std::string> (json, "title", "game");
        config.width = DOMget<int>(json, "width",500);
        config.height = DOMget<int>(json, "height",500);
        config.fullscreen = DOMget<bool>(json, "fullscreen",false);
        config.path = DOMget<std::vector<std::string> >(json, "path",{});
        config.layers = DOMget<std::vector<std::string> >(json, "layers",{});
        config.errlog = DOMget<std::string>(json, "errlog","");
        config.infolog = DOMget<std::string>(json, "infolog","");
        config.splashscreen = DOMget<bool>(json, "splashscreen",true);
        config.stderr = DOMget<bool>(json, "stderr",true);
        config.debugrender = DOMget<bool>(json, "debugrender",false);
        config.entry = DOMget<std::string>(json, "entry");
        return config;
    }
}

#endif
