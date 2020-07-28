#include "AssetSystem.hpp"

using namespace cppfs;

namespace ge{
    namespace AssetSystem{
        std::vector<std::string> g_path = {"."};
        std::map<std::string, std::unique_ptr<sf::Texture> > g_textures;
        std::map<std::string, std::unique_ptr<sf::SoundBuffer> > g_sounds;
        std::map<std::string, std::unique_ptr<sf::Font> > g_fonts;
//         std::map<std::string, std::unique_ptr<Tileset> > g_tilesets;
//         std::map<std::string, std::unique_ptr<Tilemap> > g_tilemaps;
//         std::map<std::string, std::unique_ptr<Animation> > g_animations;
//         std::map<std::string, std::unique_ptr<EventScript> > g_eventscripts;
        std::map<std::string, std::unique_ptr<Prototype> > g_prototypes;
        std::map<std::string, std::unique_ptr<Scene> > g_scenes;
        std::map<std::string, AssetInfo> g_assetinfo;
        
        // default assets
        
        sf::Texture default_texture_instance;
        sf::SoundBuffer default_sound_instance;
        sf::Font default_font_instance;
//         Tileset default_tileset_instance;
//         Tilemap default_tilemap_instance;
//         Animation default_animation_instance;
//         EventScript default_eventscript_instance;
        Prototype default_prototype_instance;
        Scene default_scene_instance;
        
        sf::Texture *default_texture=&default_texture_instance;
        sf::SoundBuffer *default_sound=&default_sound_instance;
        sf::Font *default_font=&default_font_instance;
//         Tileset *default_tileset=&default_tileset_instance;
//         Tilemap *default_tilemap=&default_tilemap_instance;
//         Animation *default_animation=&default_animation_instance;
//         EventScript *default_eventscript=&default_eventscript_instance;
        Prototype *default_prototype=&default_prototype_instance;
        Scene *default_scene=&default_scene_instance;
        
        // "private" auxiliar
        std::map <std::string, std::string> assetNameCache; 
        std::string findAsset(const std::string& name){
            auto it = assetNameCache.find(name);
            if(it != assetNameCache.end()){
                return it->second;
            }
            FileHandle file;
            for(auto it = g_path.begin(); it != g_path.end(); it++){
                FilePath fp(*it);
                file = fs::open(fp.resolve(name).path());
                if(file.exists()){
                    auto fp = FilePath(file.path()).resolved();
                    assetNameCache[name] = fp;
                    return fp;
                }
            }
            LOG_F(ERROR, "Unable to locate asset: {}", name);
            return name;
        }
        template<class T>
        T* loadAsset(const std::string& str){
            T* ret = new T;
            bool ok = ret->loadFromFile(str);
            if(!ok){
                LOG_F(ERROR, "Unable to load", str);
                delete ret;
                ret = nullptr;
            }else{
                LOG_F(INFO, "Loaded", str);
            }
            return ret;
        }
        AssetType typeFromExtension(const std::string& str){
            static std::map<std::string, AssetType> formats = {
                {".bmp", TEXTURE},
                {".png", TEXTURE},
                {".tga", TEXTURE},
                {".jpg", TEXTURE},
                {".gif", TEXTURE},
                {".psd", TEXTURE},
                {".hdr", TEXTURE},
                {".pic", TEXTURE},
                {".wav", SOUND},
                {".ogg", SOUND},
                {".flac", SOUND},
                {".ttf", FONT},
                {".cff", FONT},
                {".x11", FONT},
                {".sfnt", FONT},
                {".pfr", FONT},
                {".bdf", FONT},
                {".tileset", AssetType::TILESET},
                {".tilemap", AssetType::TILEMAP},
                {".animation", AssetType::ANIMATION},
                {".eventscript", AssetType::EVENTSCRIPT},
                {".prototype", AssetType::PROTOTYPE},
                {".scene", AssetType::SCENE}
            };
            std::string extension = str.substr(str.find_last_of('.'));
            ge::toLower(extension);
            auto it2 = formats.find(extension);
            if(it2 == formats.end()){
                LOG_F(ERROR,"Unrecognized extension \"{}\"",extension);
                return AssetType::UNRECOGNIZED;
            }else{
                return it2->second;
            }
        }
        
        // public
        const std::vector<std::string>& path(){
            return g_path;
        }
        void addPath(const std::string& p){
            g_path.push_back(p);
        }
        bool load(const std::string& name, bool find){
            bool ret;
            std::string path = find ? findAsset(name) : FilePath(name).resolved();
            LOG_SCOPE_F(INFO,"Loading %s", path.c_str());
            AssetType type = typeFromExtension(path);
            auto it = g_assetinfo.find(path);
            // if it already exists we don't load it again.
            if(it != g_assetinfo.end()){
                AssetState st = it->second.state;
                if(st == ERROR){
                    LOG_F(INFO,"Loaded with error");
                    ret = false;
                }else if(st == LOADED){
                    LOG_F(INFO,"Already loaded");
                    ret = true;
                }
            }else if(type == UNRECOGNIZED){
                AssetInfo& info = g_assetinfo[path];
                info.type = type;
                info.name = path;
                info.state = ERROR;
                ret = false;
            }else{
                AssetInfo& info = g_assetinfo[path];
                info.type = type;
                info.name = path;
                info.state = ERROR;
                switch(type){
                case TEXTURE:{
                    auto ptrtex = loadAsset<sf::Texture>(path);
                    if(ptrtex != nullptr){
                        g_textures[path] = move(std::unique_ptr<sf::Texture> (ptrtex));
                        info.state = LOADED;
                    }
                    break;
                }
                case SOUND:{
                    auto ptrsnd = loadAsset<sf::SoundBuffer>(path);
                    if(ptrsnd != nullptr){
                        g_sounds[path] = move(std::unique_ptr<sf::SoundBuffer> (ptrsnd));
                        info.state = LOADED;
                    }
                    break;
                }
                case FONT:{
                    auto ptrfnt = loadAsset<sf::Font>(path);
                    if(ptrfnt != nullptr){
                        g_fonts[path] = move(std::unique_ptr<sf::Font>(ptrfnt));
                        info.state = LOADED;
                    }
                    break;
                }
                case PROTOTYPE:{
                    auto ptrpro = loadAsset<Prototype>(path);
                    if(ptrpro != nullptr){
                        if(!ptrpro->m_nameOfBase.empty()){
                            info.dependencies.push_back(ptrpro->m_nameOfBase);
                            if(loadDependencies(path)){ // done this way to prevent cycles
                                ptrpro->m_base = getPrototype(ptrpro->m_nameOfBase);
                            }
                        }
                        g_prototypes[path] = move(std::unique_ptr<Prototype>(ptrpro));
                        info.state = LOADED;
                    }
                    break;
                }
                case SCENE:{
                    auto ptrsce = loadAsset<Scene>(path);
                    if(ptrsce != nullptr){
                        g_scenes[path] = move(std::unique_ptr<Scene>(ptrsce));
                        info.state = LOADED;
                    }
                    break;
                }
                
                default:
                    LOG_F(WARNING, "Unimplemented asset type load: {}", path);
                    break;
                }
                ret = info.state == LOADED;
            }
            return ret;
        }
        bool loadRecursively(const std::string& str){
            std::string fp = findAsset(str);
            FileHandle dir = fs::open(fp);
            LOG_SCOPE_F(INFO, "Loading from %s", fp.c_str());
            if(dir.isDirectory()){
                dir.traverse([](FileHandle& fh) -> bool {
                    if(fh.isFile()){
                        load(fh.path(), false);
                    }
                    return true;
                });
            }else{
                LOG_F(WARNING, "Not a directory!");
                return false;
            }
            return true;
        }
        std::set<std::string> unloadedDependencies(){
            std::set<std::string> unloaded;
            for(auto& info: g_assetinfo){
                std::vector<std::string>& deps = info.second.dependencies;
                for(auto& dep: deps){
                    auto it = g_assetinfo.find(dep);
                    if(it == g_assetinfo.end() || it->second.state != LOADED){
                        unloaded.insert(dep);
                    }
                }
            }
            return unloaded;
        }
        bool loadDependencies(const std::string& str){
            static std::set<std::string> fixed;
            fixed.insert(str);
            bool ret = false;
            auto it = g_assetinfo.find(str);
            if(it != g_assetinfo.end()){
                auto& info = it->second;
                ret = true;
                auto itd = info.dependencies.begin();
                while(ret && itd != info.dependencies.end()){
                    ret = load(*itd);
                    if(fixed.find(*itd) == fixed.end()){
                        ret = loadDependencies(*itd);
                    }
                    itd++;
                }
            }
            return ret;
        }
        std::vector<AssetInfo> getInfo(
            bool(*condition)(const AssetInfo&)
        ){
            std::vector<AssetInfo> ret;
            for(auto& ai: g_assetinfo){
                if(condition(ai.second)){
                    ret.push_back(ai.second);
                }
            }
            return ret;
        }
        sf::Texture* getTexture(const std::string& str){
            std::string fp = findAsset(str);
            auto it = g_textures.find(fp);
            if(it != g_textures.end()){
                g_assetinfo.at(fp).timesAccessed += 1;
                return it->second.get();
            }else{
                LOG_IF_F(ERROR, !str.empty(), "Texture {} not loaded", fp);
                return default_texture;
            }
        }
        sf::SoundBuffer* getSound(const std::string& str){
            std::string fp = findAsset(str);
            auto it = g_sounds.find(fp);
            if(it != g_sounds.end()){
                g_assetinfo.at(fp).timesAccessed += 1;
                return it->second.get();
            }else{
                LOG_IF_F(ERROR, !str.empty(), "Sound {} not loaded", str);
                return default_sound;
            }
        }
        sf::Font* getFont(const std::string& str){
            std::string fp = findAsset(str);
            auto it = g_fonts.find(fp);
            if(it != g_fonts.end()){
                return it->second.get();
            }else{
                LOG_IF_F(ERROR, !str.empty(), "Font {} not loaded", str);
                return default_font;
            }
        }
/*
        // Tileset* getTileset(const std::string& str=""){
//             auto it = g_tilesets.find(str);
//             if(it != g_tilesets.end()){
//                 return it->second.get();
//             }else{
//                 return default_tileset;
//             }
//         }
        // Tilemap* getTilemap(const std::string& str=""){
//             auto it = g_tilemaps.find(str);
//             if(it != g_tilemaps.end()){
//                 return it->second.get();
//             }else{
//                 return default_tilemap;
//             }
//         }
        // Animation* getAnimation(const std::string& str="");{
//             auto it = g_animations.find(str);
//             if(it != g_animations.end()){
//                 return it->second.get();
//             }else{
//                 return default_animation;
//             }
//         }
        // EventScript* getEventScript(const std::string& str="");{
//             auto it = g_eventscripts.find(str);
//             if(it != g_eventscripts.end()){
//                 return it->second.get();
//             }else{
//                 return default_eventscript;
//             }
//         }
*/
        Prototype* getPrototype(const std::string& str){
            std::string fp = findAsset(str);
            auto it = g_prototypes.find(fp);
            if(it != g_prototypes.end()){
                return it->second.get();
            }else{
                LOG_IF_F(ERROR, !str.empty(), "Couldn't get {}", fp);
                return default_prototype;
            }
        }

        Scene* getScene(const std::string& str){
            std::string fp = findAsset(str);
            auto it = g_scenes.find(fp);
            if(it != g_scenes.end()){
                return it->second.get();
            }else{
                LOG_IF_F(ERROR, !str.empty(), "Couldn't get {}", fp);
                return default_scene;
            }
        }

        bool setDefaultTexture(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
                auto it = g_textures.find(fp);
                if(it != g_textures.end()){
                    g_assetinfo.at(fp).timesAccessed++;
                    default_texture = it->second.get();
                }else{
                    LOG_F(ERROR,"Texture {} not available for default",fp);
                }
            }
            return ok;
        }
        bool setDefaultSound(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
                auto it = g_sounds.find(fp);
                if(it != g_sounds.end()){
                    g_assetinfo.at(fp).timesAccessed++;
                    default_sound = it->second.get();
                }else{
                    LOG_F(ERROR,"Sound {} not available for default",fp);
                }
            }
            return ok;
        }
        bool setDefaultFont(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
                auto it = g_fonts.find(fp);
                if(it != g_fonts.end()){
                    g_assetinfo.at(fp).timesAccessed++;
                    default_font = it->second.get();
                }else{
                    LOG_F(ERROR,"Font {} not available for default",fp);
                }
            }
            return ok;
        }
        bool setDefaultTileset(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
//                 auto it = g_tilesets.find(fp);
//                 if(it != g_tilesets.end()){
//                     g_assetinfo.at(fp).timesAccessed++;
//                     default_tileset = it->second.get();
//                 }else{
//                     LOG_F(ERROR,"Texture {} not available for default",fp);
//                 }
            }
            return ok;
        }
        bool setDefaultTileMap(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
//                 auto it = g_tilemaps.find(fp);
//                 if(it != g_tilemaps.end()){
//                     g_assetinfo.at(fp).timesAccessed++;
//                     default_tilemap = it->second.get();
//                 }else{
//                     LOG_F(ERROR,"Texture {} not available for default",fp);
//                 }
            }
            return ok;
        }
        bool setDefaultAnimation(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
//                 auto it = g_animations.find(fp);
//                 if(it != g_animations.end()){
//                      g_assetinfo.at(fp).timesAccessed++;
//                     default_animation = it->second.get();
//                 }else{
//                     LOG_F(ERROR,"Texture {} not available for default",fp);
//                 }
            }
            return ok;
        }
        bool setDefaultEventScript(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
//                 auto it = g_eventscripts.find(fp);
//                 if(it != g_eventscripts.end()){
//                     g_assetinfo.at(fp).timesAccessed++;
//                     default_eventscript = it->second.get();
//                 }else{
//                     LOG_F(ERROR,"Texture {} not available for default",fp);
//                 }
            }
            return ok;
        }
        bool setDefaultProtype(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
                auto it = g_prototypes.find(fp);
                if(it != g_prototypes.end()){
                    g_assetinfo.at(fp).timesAccessed++;
                    default_prototype = it->second.get();
                }else{
                    LOG_F(ERROR,"Texture {} not available for default",fp);
                }
            }
            return ok;
        }
        bool setDefaultScene(const std::string& str){
            std::string fp = findAsset(str);
            bool ok = load(fp, false);
            if(ok){
                auto it = g_scenes.find(fp);
                if(it != g_scenes.end()){
                    g_assetinfo.at(fp).timesAccessed++;
                    default_scene = it->second.get();
                }else{
                    LOG_F(ERROR,"Texture {} not available for default",fp);
                }
            }
            return ok;
        }
    }
}
