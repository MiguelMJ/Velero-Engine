/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the namespace
 * with the functions of the Asset System.
 */
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

#include "log.hpp"
#include "stringmanip.hpp"

namespace ge{
    /**
     * @brief Different types of assets managed by the engine.
     */
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
    /**
     * @brief Possible states of a tracked assets.
     */
    typedef enum{
        Unloaded, ///< Asset tracked but not loaded.
        Loaded,   ///< Asset succesfully loaded.
        Error     ///< Asset not found or impossible to load.
    } AssetState;
    /**
     * @brief Information of a tracked asset.
     */
    typedef struct{
        AssetType type;
        std::string name;
        AssetState state;
        std::vector<std::string> dependencies;
        int timesAccessed;
    } AssetInfo;
    /**
     * @brief System to load and access to assets.
     * @details Assets are identified by its path, be it absolute or
     * relative. They can be used interchangeably. For example, if
     * you load "/project/absolute/path/assets/images/player.png" you can 
     * then access it just by "assets/images/player.png". Also, this 
     * system uses a list of possible relative paths, so if you add, lets 
     * say, "assets/images" to the list, you would be able to load and 
     * get the same asset just with "player.png".
     */
    namespace AssetSystem{
        /**
         * @brief Get the list of paths used to do the relative path 
         * searches.
         * @see addPath
         */
        const std::vector<std::string>& path();
        /**
         * @brief Add a path to the list used to do the relative path
         * searches.
         * @details Both relative and absolute paths are accepted.
         * @see path
         */
        void addPath(const std::string&);
        
        /**
         * @brief Load an asset, given its path.
         * @param path Path to the asset.
         * @param find Tells the function to look for the asset in the 
         * relative paths. If it is set to false, the @p path must be 
         * either absolute or relative to the current working directory.
         * @returns True if the load was succesfull. Reloading an already 
         * failed asset will return false immediatly.
         * @see loadRecursively loadDependencies
         */
        bool load(const std::string& path, bool find=true);
        
        /**
         * @brief Load all assets from a directory tree.
         * @details Symbolic links have not been tested. Cycles produced 
         * by such links might or might not be a problem
         * @param directory Directory to load from.
         * @returns True if @p directory is actually a directory.
         * @see load
         */
        bool loadRecursively(const std::string& directory);
        
        /**
         * @brief Gets the assets required by other assets but not 
         * loaded.
         * @returns Set of unloaded assets, required by other assets.
         * @see loadDependencies
         */
        std::set<std::string> unloadedDependencies();
        
        /**
         * @brief Load the dependencies of certain asset.
         * @details Circular dependencies are ignored. If a dependency 
         * fails to load, the rest are left unloaded.
         * @param path Path of the asset to load the dependecies of.
         * @returns True if the dependencies were succesfully loaded.
         * @see load
         */
        bool loadDependencies(const std::string&);
        
        /**
         * @brief Retrieve a list with the information of the assets
         * that fulfill certain condition.
         * @details By default, if no condition is specified, the full
         * list of assets is retreived.
         * @param condition Function that takes the asset information
         * as argument and returns a boolean value.
         * @returns List with the information of the assets that fulfill
         * the @p condition.
         */
        std::vector<AssetInfo> getInfo(
            bool(*condition)(const AssetInfo&) =
            [](const AssetInfo&)->bool{return true;}
        );
        
        /**
         * @brief Get a loaded Texture.
         * @details If the texture is not loaded, then the default one is 
         * retreived.
         * @param path Path to the texture file.
         * @returns Either the specified texture or the default one.
         * @see load setDefaultTexture
         */
        sf::Texture* getTexture(const std::string& path="");
        
        /**
         * @brief Get a loaded SoundBuffer.
         * @details If the sound buffer is not loaded, then the default 
         * one is retreived.
         * @param path Path to the sound file.
         * @returns Either the specified sound buffer or the default one.
         * @see load setDefaultSound
         */
        sf::SoundBuffer* getSound(const std::string& path="");
        
        /**
         * @brief Get a loaded Font.
         * @details If the font is not loaded, then the default one is 
         * retreived.
         * @param path Path to the font file.
         * @returns Either the specified font or the default one.
         * @see load setDefaultFont
         */
        sf::Font* getFont(const std::string& path="");
        
        /*
         * @brief Get a loaded Tileset.
         * @details If the tileset is not loaded, then the default one is 
         * retreived.
         * @param path Path to the tileset file.
         * @returns Either the specified tileset or the default one.
         * @see load setDefaultTileset
         */
        // Tileset* getTileset(const std::string& path="");
        
        /*
         * @brief Get a loaded Tilemap.
         * @details If the tilemap is not loaded, then the default one is 
         * retreived.
         * @param path Path to the tilemap file.
         * @returns Either the specified tilemap or the default one.
         * @see load setDefaultTilemap
         */
        // Tilemap* getTilemap(const std::string& path="");
        
        /*
         * @brief Get a loaded Animation.
         * @details If the animation is not loaded, then the default one 
         * is retreived.
         * @param path Path to the animation file.
         * @returns Either the specified animation or the default one.
         * @see load setDefaultAnimation
         */
        // Animation* getAnimation(const std::string& path="");
        
        /*
         * @brief Get a loaded Eventscript.
         * @details If the event script is not loaded, then the default 
         * one is retreived.
         * @param path Path to the event script file.
         * @returns Either the specified event script or the default one.
         * @see load setDefaultEventScript
         */
        // EventScript* getEventScript(const std::string& path="");
        
        /*
         * @brief Get a loaded Protoype.
         * @details If the prototype is not loaded, then the default one 
         * is retreived.
         * @param path Path to the prototype file.
         * @returns Either the specified prototype or the default one.
         * @see load setDefaultPrototype
         */
        // Protoype* getPrototype(const std::string& path="");
        
        /*
         * @brief Get a loaded Scene.
         * @details If the scene is not loaded, then the default one is 
         * retreived.
         * @param path Path to the scene file.
         * @returns Either the specified scene or the default one.
         * @see load setDefaultScene
         */
        // Scene* getScene(const std::string& path="");
        
        /**
         * @brief Set the default texture.
         * @details If the @p path is correct, this texture will be 
         * returned when a requested texture doesn't exist.
         * @param path Path of the texture file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultTexture(const std::string& path);
        
        /**
         * @brief Set the default sound buffer.
         * @details If the @p path is correct, this sound buffer will be 
         * returned when a requested sound buffer doesn't exist.
         * @param path Path of the sound buffer file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultSound(const std::string& path);
        
        /**
         * @brief Set the default font.
         * @details If the @p path is correct, this font will be 
         * returned when a requested font doesn't exist.
         * @param path Path of the font file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultFont(const std::string& path);
        
        /**
         * @brief Set the default tileset.
         * @details If the @p path is correct, this tileset will be 
         * returned when a requested tileset doesn't exist.
         * @param path Path of the tileset file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultTileset(const std::string& path);
        
        /**
         * @brief Set the default tilemap.
         * @details If the @p path is correct, this tilemap will be 
         * returned when a requested tilemap doesn't exist.
         * @param path Path of the tilemap file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultTileMap(const std::string& path);
        
        /**
         * @brief Set the default animation.
         * @details If the @p path is correct, this animation will be 
         * returned when a requested animation doesn't exist.
         * @param path Path of the animation file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultAnimation(const std::string& path);
        
        /**
         * @brief Set the default event script.
         * @details If the @p path is correct, this event script will be 
         * returned when a requested event script doesn't exist.
         * @param path Path of the event script file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultEventScript(const std::string& path);
        
        /**
         * @brief Set the default prototype.
         * @details If the @p path is correct, this prototype will be 
         * returned when a requested prototype doesn't exist.
         * @param path Path of the prototype file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultProtype(const std::string& path);
        
        /**
         * @brief Set the default scene.
         * @details If the @p path is correct, this scene will be 
         * returned when a requested scene doesn't exist.
         * @param path Path of the scene file.
         * @returns True if the operation was successful.
         * @see getTexture
         */
        bool setDefaultScene(const std::string&  path);
    }
}

#endif
