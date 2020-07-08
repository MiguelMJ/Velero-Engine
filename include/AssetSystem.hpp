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
        int timesAccessed;
    } AssetInfo;
    namespace AssetSystem{
        const std::vector<std::string>& path();
        void addPath(const std::string&);
        bool load(const std::string& name, bool find=true);
        bool loadRecursively(const std::string&);
        std::set<std::string> unloadedDependencies();
        bool loadDependencies(const std::string&);
        std::vector<AssetInfo> getInfo(
            bool(*condition)(const AssetInfo&) =
            [](const AssetInfo&)->bool{return true;}
        );
        sf::Texture* getTexture(const std::string& str="");
        sf::SoundBuffer* getSound(const std::string& str="");
        sf::Font* getFont(const std::string& str="");
        // Tileset* getTileset(const std::string& str="");
        // Tilemap* getTilemap(const std::string& str="");
        // Animation* getAnimation(const std::string& str="");
        // EventScript* getEventScript(const std::string& str="");
        // Protoype* getPrototype(const std::string& str="");
        // Scene* getScene(const std::string& str="");
        bool setDefaultTexture(const std::string&);
        bool setDefaultSound(const std::string&);
        bool setDefaultFont(const std::string&);
        bool setDefaultTileset(const std::string&);
        bool setDefaultTileMap(const std::string&);
        bool setDefaultAnimation(const std::string&);
        bool setDefaultEventScript(const std::string&);
        bool setDefaultProtype(const std::string&);
        bool setDefaultScene(const std::string&);
    }
}

#endif
