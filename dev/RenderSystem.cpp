#include "RenderSystem.hpp"


namespace ge{
    
    namespace RenderSystem{
        std::vector<std::unique_ptr<Layer>> g_layers;
        std::map<std::string, size_t> g_layerIndex;
        std::map<size_t, std::unique_ptr<Layer>> g_lockedLayers;
        std::vector<bool> g_layerIsLocked;
        std::vector<bool> g_layerIsVisible;
        std::vector<sf::RenderStates> g_renderStates;
        sf::View view;
        
        /*
         * public functions
         */
        Layer* pushLayer(std::string str){
            g_layerIndex[str] = g_layers.size();
            auto ptr = new DynamicLayer();
            g_layers.emplace_back(ptr);
            g_layerIsLocked.push_back(false);
            g_layerIsVisible.push_back(true);
            g_renderStates.push_back(sf::RenderStates());
            return ptr;
        }
        LightingLayer* pushLightingLayer(std::string str){
            g_layerIndex[str] = g_layers.size();
            auto ptr = new LightingLayer();
            g_layers.emplace_back(ptr);
            g_layerIsLocked.push_back(false);
            g_layerIsVisible.push_back(true);
            g_renderStates.push_back(sf::RenderStates());
            return ptr;
        }
        Layer* duplicateLayer(size_t id, std::string str){
            if(isLightingLayer(id)){
                return duplicateLightingLayer(id, str);
            }else if(! g_layerIsLocked[id]){
                auto ptrll = static_cast<DynamicLayer*> (g_layers[id].get());
                Layer* newll = new DynamicLayer(*ptrll);
                g_layers.emplace_back(newll);
                g_layerIsLocked.push_back(false);
                g_layerIsVisible.push_back(true);
                g_renderStates.push_back(
                    sf::RenderStates(g_renderStates[id])
                );
                g_layerIndex[str] = g_layers.size();
                return newll;
            }
        }
        LightingLayer* duplicateLightingLayer(size_t id, std::string str){
            auto ptrll = static_cast<LightingLayer*> (g_layers[id].get());
            LightingLayer* newll = new LightingLayer(*ptrll);
            g_layers.emplace_back(newll);
            g_layerIsLocked.push_back(false);
            g_layerIsVisible.push_back(true);
            g_renderStates.push_back(
                sf::RenderStates(g_renderStates[id])
            );
            g_layerIndex[str] = g_layers.size();
            return newll;
        }
        void moveLayer(size_t id, int d){
            int dest = id + d;
            if(d != 0 && dest >= 0 && dest < g_layers.size()){
                auto tmp = move(g_layers[id]);
                g_layers.erase(g_layers.begin()+id);
                g_layers.insert(g_layers.begin()+dest, move(tmp));
                bool tmpb = g_layerIsLocked[id];
                g_layerIsLocked.erase(g_layerIsLocked.begin()+id);
                g_layerIsLocked.insert(g_layerIsLocked.begin()+dest, tmpb);
                tmpb = g_layerIsVisible[id];
                g_layerIsVisible.erase(g_layerIsVisible.begin()+id);
                g_layerIsVisible.insert(g_layerIsVisible.begin()+dest, tmpb);
                auto tmprs = g_renderStates[id];
                g_renderStates.erase(g_renderStates.begin()+id);
                g_renderStates.insert(g_renderStates.begin()+dest,tmprs);
                
            }
            if(dest < id){ // moved down
                for(auto& ip: g_layerIndex){
                    if(ip.second == id){
                        ip.second = dest;
                    }else if(ip.second >= dest && ip.second < id){
                        ip.second++;
                    }
                }
            }else{ // moved up
                for(auto& ip: g_layerIndex){
                    if(ip.second == id){
                        ip.second = dest;
                    }else if(ip.second > id && ip.second <= dest){
                        ip.second--;
                    }
                }
            }
        }
        void deleteLayer (std::string str){
            auto it  = g_layerIndex.find(str);
            if(it != g_layerIndex.end()){
                int id = it->second;
                // erase layer
                g_layers.erase(g_layers.begin()+id);
                // erase dynamic information if it's locked
                if(g_layerIsLocked[id]){
                    g_lockedLayers.erase(g_lockedLayers.find(id));
                }
                // erase layer additional information
                g_layerIndex.erase(it);
                g_layerIsLocked.erase(g_layerIsLocked.begin()+id);
                g_layerIsVisible.erase(g_layerIsVisible.begin()+id);
                g_renderStates.erase(g_renderStates.begin()+id);
                // update index
                for(auto& ie: g_layerIndex){
                    if(ie.second > id){
                        ie.second--;
                    }
                }
                std::map<size_t, std::unique_ptr<Layer>> tmp;
                for(auto& ie: g_lockedLayers){
                    int newid = ie.first > id ? ie.first-1: ie.first;
                    tmp[newid] = move(ie.second);
                }
                g_lockedLayers.swap(tmp);
            }
        }
        size_t layerCount(){
            return g_layers.size();
        }
        void setLocked(size_t id, bool l){
            if(l && !g_layerIsLocked[id]){ // lock
                StaticLayer* ptrLocked;
                auto ptrUnlocked = g_layers[id].get();
                if(typeid(*ptrUnlocked) == typeid(LightingLayer)){
                    ptrLocked = new StaticLayer( *static_cast<LightingLayer*>(ptrUnlocked) );
                }else if(typeid(*ptrUnlocked) == typeid(DynamicLayer)){
                    ptrLocked = new StaticLayer( *static_cast<DynamicLayer*>(ptrUnlocked) );
                }
                g_lockedLayers[id] = move(g_layers[id]);
                g_layers[id] = move(std::unique_ptr<Layer>(ptrLocked));
                g_layerIsLocked[id] = true;
            }else if(!l && g_layerIsLocked[id]){ // unlock
                auto it = g_lockedLayers.find(id);
                g_layers[id] = move(it->second);
                g_lockedLayers.erase(it);
                g_layerIsLocked[id] = false;
            }else{
                
            }
        }
        void setVisible (size_t id, bool v){
            g_layerIsVisible[id] = v;
        }
        void setVisible (const Renderable* r, bool v){
            for(auto& l : g_layers){
                l->setVisible(r,v);
            }
        }
        sf::RenderStates& getRenderState (size_t id){
            return g_renderStates[id];
        }
        Layer* getLayer(std::string str){
            Layer* ret = nullptr;
            auto it = g_layerIndex.find(str);
            if(it != g_layerIndex.end()){
                ret = g_layers[it->second].get();
            }
            return ret;
        }
        Layer* getLayer (size_t id){
            return g_layers[id].get();
        }
        LightingLayer* getLightingLayer(std::string str){
            return static_cast<LightingLayer*> (getLayer(str));
        }
        LightingLayer* getLightingLayer (size_t id){
            return static_cast<LightingLayer*> (getLayer(id));
        }
        size_t getLayerId (std::string str){
            size_t ret = -1;
            auto it = g_layerIndex.find(str);
            if(it != g_layerIndex.end()){
                ret = it->second;
            }
            return ret;
        }
        bool isLightingLayer (size_t id){
            return typeid(*g_layers[id]) == typeid(LightingLayer);
        }
        void draw(sf::RenderTarget& t){
            for(int i=0; i < g_layers.size(); i++){
                if(g_layerIsVisible[i]){
                    t.draw(*g_layers[i], g_renderStates[i]);
                }
            }
        }
    }
}
