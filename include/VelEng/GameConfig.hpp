#ifndef __VELERO_GAME_CONFIG_HPP__  
#define __VELERO_GAME_CONFIG_HPP__  

#include <string>
#include <vector>

namespace ven{
    struct GameConfig{
        std::string title;
        int width;
        int height;
        bool fullscreen;
        std::vector<std::string> path;
        std::vector<std::string> layers;
        bool debugrender;
        std::string errlog;
        std::string infolog;
        std::string entry;
        bool splashscreen;
    };
}

#endif
