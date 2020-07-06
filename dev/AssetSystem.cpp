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
//         std::map<std::string, std::unique_ptr<Prototype> > g_prototypes;
//         std::map<std::string, std::unique_ptr<Scene> > g_scenes;
        std::map<std::string, AssetInfo> g_assetinfo;
        
        // default values
        
        sf::Texture default_texture;
        sf::SoundBuffer default_sound;
        sf::Font default_font;
//         Tileset default_tileset;
//         Tilemap default_tilemap;
//         Animation default_animation;
//         EventScript default_eventscript;
//         Prototype default_prototype;
//         Scene default_scene;
        
        // "private" auxiliar
        template<class T>
        T* loadAsset(const std::string& str){
            T* ret = nullptr;
            bool ok = false;
            FilePath path(str);
            if(path.pointsToContent()){
                ret = new T;
                if(path.isAbsolute()){
                    ok = ret->loadFromFile(path.fullPath());
                }else{
                    for(auto& p: g_path){
                        FilePath src(p);
                        ok = ret->loadFromFile(src.resolve(str).fullPath());
                        if(ok){
                            break;
                        }
                    }
                }
                if(!ok){
                    delete ret;
                    ret = nullptr;
                }
            }
            return ret;
        }
        AssetType typeFromExtension(const std::string& str){
            static std::map<std::string, AssetType> formats = {
                {".bmp", Texture},
                {".png", Texture},
                {".tga", Texture},
                {".jpg", Texture},
                {".gif", Texture},
                {".psd", Texture},
                {".hdr", Texture},
                {".pic", Texture},
                {".wav", Sound},
                {".ogg", Sound},
                {".flac", Sound},
                {".ttf", Font},
                {".cff", Font},
                {".x11", Font},
                {".sfnt", Font},
                {".pfr", Font},
                {".bdf", Font},
                {".tileset", AssetType::Tileset},
                {".tilemap", AssetType::Tilemap},
                {".animation", AssetType::Animation},
                {".eventscript", AssetType::EventScript},
                {".prototype", AssetType::Prototype},
                {".scene", AssetType::Scene}
            };
            std::string extension = FilePath(str).extension();
            ge::toLower(extension);
            auto it2 = formats.find(extension);
            if(it2 == formats.end()){
                return AssetType::Unrecognized;
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
        bool load(const std::string& str){
            bool ret;
            AssetType type = typeFromExtension(str);
            auto it = g_assetinfo.find(str);
            if(it != g_assetinfo.end()){
                AssetState st = it->second.state;
                if(st == Error){
                    ret = false;
                }else if(st == Loaded){
                    ret = true;
                }
            }else if(type == Unrecognized){
                AssetInfo& info = g_assetinfo[str];
                info.type = type;
                info.name = str;
                info.state = Error;
                ret = false;
            }else{
                AssetInfo& info = g_assetinfo[str];
                info.type = type;
                info.name = str;
                info.state = Error;
                switch(type){
                case Texture:{
                    auto ptrtex = loadAsset<sf::Texture>(str);
                    if(ptrtex != nullptr){
                        g_textures[str] = move(std::unique_ptr<sf::Texture> (ptrtex));
                        info.state = Loaded;
                    }
                    break;
                }
                case Sound:{
                    auto ptrsnd = loadAsset<sf::SoundBuffer>(str);
                    if(ptrsnd != nullptr){
                        g_sounds[str] = move(std::unique_ptr<sf::SoundBuffer> (ptrsnd));
                        info.state = Loaded;
                    }
                    break;
                }
                case Font:{
                    auto ptrfnt = loadAsset<sf::Font>(str);
                    if(ptrfnt != nullptr){
                        g_fonts[str] = move(std::unique_ptr<sf::Font>(ptrfnt));
                        info.state = Loaded;
                    }
                    break;
                }
                default:
                    // error
                    break;
                }
                ret = info.state == Loaded;
            }
            return ret;
        }
        bool loadRecursively(const std::string& str){
            FileHandle dir = fs::open(str);
            dir.traverse([](FileHandle& fh) -> bool {
                load(fh.path());
                return true;
            }, [](FileHandle& dir) -> bool {
                return true;
            });
        }
        std::set<std::string> unloadedDependencies(const std::string& str){
            std::set<std::string> unloaded;
            for(auto& info: g_assetinfo){
                std::vector<std::string>& deps = info.second.dependencies;
                for(auto& dep: deps){
                    auto it = g_assetinfo.find(dep);
                    if(it == g_assetinfo.end() || it->second.state != Loaded){
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
            auto it = g_textures.find(str);
            if(it != g_textures.end()){
                return it->second.get();
            }else{
                return &default_texture;
            }
        }
        sf::SoundBuffer* getSound(const std::string& str){
            auto it = g_sounds.find(str);
            if(it != g_sounds.end()){
                return it->second.get();
            }else{
                return &default_sound;
            }
        }
        sf::Font* getFont(const std::string& str){
            auto it = g_fonts.find(str);
            if(it != g_fonts.end()){
                return it->second.get();
            }else{
                return &default_font;
            }
        }
/*
        // Tileset* getTileset(const std::string& str=""){
//             auto it = g_tilesets.find(str);
//             if(it != g_tilesets.end()){
//                 return it->second.get();
//             }else{
//                 return &default_tileset;
//             }
//         }
        // Tilemap* getTilemap(const std::string& str=""){
//             auto it = g_tilemaps.find(str);
//             if(it != g_tilemaps.end()){
//                 return it->second.get();
//             }else{
//                 return &default_tilemap;
//             }
//         }
        // Animation* getAnimation(const std::string& str="");{
//             auto it = g_animations.find(str);
//             if(it != g_animations.end()){
//                 return it->second.get();
//             }else{
//                 return &default_animation;
//             }
//         }
        // EventScript* getEventScript(const std::string& str="");{
//             auto it = g_eventscripts.find(str);
//             if(it != g_eventscripts.end()){
//                 return it->second.get();
//             }else{
//                 return &default_eventscript;
//             }
//         }
        // Protoype* getPrototype(const std::string& str="");{
//             auto it = g_prototypes.find(str);
//             if(it != g_prototypes.end()){
//                 return it->second.get();
//             }else{
//                 return &default_prototype;
//             }
//         }
        // Scene* getScene(const std::string& str="");{
//             auto it = g_scenes.find(str);
//             if(it != g_scenes.end()){
//                 return it->second.get();
//             }else{
//                 return &default_scene;
//             }
//         }
*/
        void setDefaultTexture(const std::string& str){
            if(load(str)){
                auto it = g_textures.find(str);
                if(it != g_textures.end()){
                    default_texture = *it->second.get();
                }
            }
        }
        void setDefaultSound(const std::string& str){
            if(load(str)){
                auto it = g_sounds.find(str);
                if(it != g_sounds.end()){
                    default_sound = *it->second.get();
                }
            }
        }
        void setDefaultFont(const std::string& str){
            if(load(str)){
                auto it = g_fonts.find(str);
                if(it != g_fonts.end()){
                    default_font = *it->second.get();
                }
            }
        }
        void setDefaultTileset(const std::string& str){
//                 auto it = g_tilesets.find(str);
//                 if(it != g_tilesets.end()){
//                     default_tileset = *it->second.get();
//                 }
        }
        void setDefaultTileMap(const std::string& str){
//                 auto it = g_tilemaps.find(str);
//                 if(it != g_tilemaps.end()){
//                     default_tilemap = *it->second.get();
//                 }
        }
        void setDefaultAnimation(const std::string& str){
//                 auto it = g_animations.find(str);
//                 if(it != g_animations.end()){
//                     default_animation = *it->second.get();
//                 }
        }
        void setDefaultEventScript(const std::string& str){
//                 auto it = g_eventscripts.find(str);
//                 if(it != g_eventscripts.end()){
//                     default_eventscript = *it->second.get();
//                 }
        }
        void setDefaultProtype(const std::string& str){
//                 auto it = g_prototypes.find(str);
//                 if(it != g_prototypes.end()){
//                     default_prototype = *it->second.get();
//                 }
        }
        void setDefaultScene(const std::string& str){
//                 auto it = g_scenes.find(str);
//                 if(it != g_scenes.end()){
//                     default_scene = *it->second.get();
//                 }
        }
    }
}
