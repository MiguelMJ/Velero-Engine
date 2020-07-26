#include "RenderSystem.hpp"


namespace ge{
    Layer* RenderSystem::pushLayer(const std::string& str){
        m_layerIndex[str] = m_layers.size();
        auto ptr = new DynamicLayer();
        m_layers.emplace_back(ptr);
        m_layerIsLocked.push_back(false);
        m_layerIsVisible.push_back(true);
        m_renderStates.push_back(sf::RenderStates());
        return ptr;
    }
    LightingLayer* RenderSystem::pushLightingLayer(const std::string& str){
        m_layerIndex[str] = m_layers.size();
        auto ptr = new LightingLayer();
        m_layers.emplace_back(ptr);
        m_layerIsLocked.push_back(false);
        m_layerIsVisible.push_back(true);
        m_renderStates.push_back(sf::RenderStates());
        return ptr;
    }
    Layer* RenderSystem::duplicateLayer(size_t id, const std::string& str){
        if(isLightingLayer(id)){
            return duplicateLightingLayer(id, str);
        }else if(! m_layerIsLocked[id]){
            auto ptrll = static_cast<DynamicLayer*> (m_layers[id].get());
            Layer* newll = new DynamicLayer(*ptrll);
            m_layers.emplace_back(newll);
            m_layerIsLocked.push_back(false);
            m_layerIsVisible.push_back(true);
            m_renderStates.push_back(
                sf::RenderStates(m_renderStates[id])
            );
            m_layerIndex[str] = m_layers.size();
            return newll;
        }
    }
    LightingLayer* RenderSystem::duplicateLightingLayer(size_t id, const std::string& str){
        auto ptrll = static_cast<LightingLayer*> (m_layers[id].get());
        LightingLayer* newll = new LightingLayer(*ptrll);
        m_layers.emplace_back(newll);
        m_layerIsLocked.push_back(false);
        m_layerIsVisible.push_back(true);
        m_renderStates.push_back(
            sf::RenderStates(m_renderStates[id])
        );
        m_layerIndex[str] = m_layers.size();
        return newll;
    }
    void RenderSystem::moveLayer(size_t id, int d){
        int dest = id + d;
        if(d != 0 && dest >= 0 && dest < m_layers.size()){
            auto tmp = move(m_layers[id]);
            m_layers.erase(m_layers.begin()+id);
            m_layers.insert(m_layers.begin()+dest, move(tmp));
            bool tmpb = m_layerIsLocked[id];
            m_layerIsLocked.erase(m_layerIsLocked.begin()+id);
            m_layerIsLocked.insert(m_layerIsLocked.begin()+dest, tmpb);
            tmpb = m_layerIsVisible[id];
            m_layerIsVisible.erase(m_layerIsVisible.begin()+id);
            m_layerIsVisible.insert(m_layerIsVisible.begin()+dest, tmpb);
            auto tmprs = m_renderStates[id];
            m_renderStates.erase(m_renderStates.begin()+id);
            m_renderStates.insert(m_renderStates.begin()+dest,tmprs);
            
        }
        if(dest < id){ // moved down
            for(auto& ip: m_layerIndex){
                if(ip.second == id){
                    ip.second = dest;
                }else if(ip.second >= dest && ip.second < id){
                    ip.second++;
                }
            }
        }else{ // moved up
            for(auto& ip: m_layerIndex){
                if(ip.second == id){
                    ip.second = dest;
                }else if(ip.second > id && ip.second <= dest){
                    ip.second--;
                }
            }
        }
    }
    void RenderSystem::deleteLayer (const std::string& str){
        auto it  = m_layerIndex.find(str);
        if(it != m_layerIndex.end()){
            int id = it->second;
            // erase layer
            m_layers.erase(m_layers.begin()+id);
            // erase dynamic information if it's locked
            if(m_layerIsLocked[id]){
                m_lockedLayers.erase(m_lockedLayers.find(id));
            }
            // erase layer additional information
            m_layerIndex.erase(it);
            m_layerIsLocked.erase(m_layerIsLocked.begin()+id);
            m_layerIsVisible.erase(m_layerIsVisible.begin()+id);
            m_renderStates.erase(m_renderStates.begin()+id);
            // update index
            for(auto& ie: m_layerIndex){
                if(ie.second > id){
                    ie.second--;
                }
            }
            std::map<size_t, std::unique_ptr<Layer>> tmp;
            for(auto& ie: m_lockedLayers){
                int newid = ie.first > id ? ie.first-1: ie.first;
                tmp[newid] = move(ie.second);
            }
            m_lockedLayers.swap(tmp);
        }
    }
    size_t RenderSystem::layerCount() const{
        return m_layers.size();
    }
    void RenderSystem::setLocked(size_t id, bool l){
        if(l && !m_layerIsLocked[id]){ // lock
            StaticLayer* ptrLocked;
            auto ptrUnlocked = m_layers[id].get();
            if(typeid(*ptrUnlocked) == typeid(LightingLayer)){
                ptrLocked = new StaticLayer( *static_cast<LightingLayer*>(ptrUnlocked) );
            }else if(typeid(*ptrUnlocked) == typeid(DynamicLayer)){
                ptrLocked = new StaticLayer( *static_cast<DynamicLayer*>(ptrUnlocked) );
            }
            m_lockedLayers[id] = move(m_layers[id]);
            m_layers[id] = move(std::unique_ptr<Layer>(ptrLocked));
            m_layerIsLocked[id] = true;
        }else if(!l && m_layerIsLocked[id]){ // unlock
            auto it = m_lockedLayers.find(id);
            m_layers[id] = move(it->second);
            m_lockedLayers.erase(it);
            m_layerIsLocked[id] = false;
        }else{
            
        }
    }
    void RenderSystem::setVisible (size_t id, bool v){
        m_layerIsVisible[id] = v;
    }
    void RenderSystem::setVisible (const Renderable* r, bool v){
        for(auto& l : m_layers){
            l->setVisible(r,v);
        }
    }
    sf::RenderStates& RenderSystem::getRenderState (size_t id){
        return m_renderStates.at(id);
    }
    Layer* RenderSystem::getLayer(const std::string& str) const{
        Layer* ret = nullptr;
        auto it = m_layerIndex.find(str);
        if(it != m_layerIndex.end()){
            ret = m_layers[it->second].get();
        }
        return ret;
    }
    Layer* RenderSystem::getLayer (size_t id) const{
        return m_layers[id].get();
    }
    LightingLayer* RenderSystem::getLightingLayer(const std::string& str) const{
        return static_cast<LightingLayer*> (getLayer(str));
    }
    LightingLayer* RenderSystem::getLightingLayer (size_t id) const{
        return static_cast<LightingLayer*> (getLayer(id));
    }
    size_t RenderSystem::getLayerId (const std::string& str) const{
        size_t ret = -1;
        auto it = m_layerIndex.find(str);
        if(it != m_layerIndex.end()){
            ret = it->second;
        }
        return ret;
    }
    bool RenderSystem::isLightingLayer (size_t id) const{
        return typeid(*m_layers[id]) == typeid(LightingLayer);
    }
    void RenderSystem::draw(sf::RenderTarget& t){
        t.setView(m_view);
        for(int i=0; i < m_layers.size(); i++){
            if(m_layerIsVisible[i]){
                t.draw(*m_layers[i], m_renderStates[i]);
            }
        }
    }
}
