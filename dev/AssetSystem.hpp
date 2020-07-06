#ifndef __ASSETSYSTEM_HPP__
#define __ASSETSYSTEM_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>
#include <cppfs/FilePath.h>

#include <vector>
#include <map>
#include <string>
#include <exception>
#include <memory>



#include "geutil.hpp"

namespace ge{
    typedef enum {
        Texture,
        Sound,
        Font,
        Tileset,
        Tilemap,
        Animation,
        EventScript,
        Prototype,
        Scene,
        Unrecognized
    } AssetType;
    typedef enum{
        Unloaded,
        Loaded,
        Error
    } AssetState;
    typedef struct{
        AssetType type;
        std::string name;
        AssetState state;
        std::vector<std::string> dependencies;
        int timesAccesed;
    } AssetInfo;
    namespace AssetSystem{
        void addPath(const std::string&);
        bool load(const std::string&);
        bool loadRecursively(const std::string&);
        bool checkDependencies(const std::string&);
        bool checkAllDependencies();
        bool fixDependencies(const std::string&);
        bool fixAllDependencies();
        std::vector<AssetInfo> getUnused();
        sf::Texture* getTexture(const std::string& str="");
        sf::SoundBuffer* getSound(const std::string& str="");
        sf::Font* getFont(const std::string& str="");
        // Tileset* getTileset(const std::string& str="");
        // Tilemap* getTilemap(const std::string& str="");
        // Animation* getAnimation(const std::string& str="");
        // EventScript* getEventScript(const std::string& str="");
        // Protoype* getPrototype(const std::string& str="");
        // Scene* getScene(const std::string& str="");
        void setDefaultTexture(const std::string&);
        void setDefaultSound(const std::string&);
        void setDefaultFont(const std::string&);
        void setDefaultTileset(const std::string&);
        void setDefaultTileMap(const std::string&);
        void setDefaultAnimation(const std::string&);
        void setDefaultEventScript(const std::string&);
        void setDefaultProtype(const std::string&);
        void setDefaultScene(const std::string&);
    }
}

#endif
