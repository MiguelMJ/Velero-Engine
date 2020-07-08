#ifndef __RENDERSYSTEM_HPP__
#define __RENDERSYSTEM_HPP__

#include <vector>
#include <map>
#include <memory>
#include <typeinfo>

#include "SFML/Graphics.hpp"

#include "DynamicLayer.hpp"
#include "StaticLayer.hpp"
#include "LightingLayer.hpp"

namespace ge{
    class RenderSystem{
    private:
        std::vector<std::unique_ptr<Layer>> m_layers;
        std::map<std::string, size_t> m_layerIndex;
        std::map<size_t, std::unique_ptr<Layer>> m_lockedLayers;
        std::vector<bool> m_layerIsLocked;
        std::vector<bool> m_layerIsVisible;
        std::vector<sf::RenderStates> m_renderStates;
    public:
        sf::View view;
        
        // extern sf::View view;
        Layer* pushLayer(std::string);
        LightingLayer* pushLightingLayer(std::string);
        Layer* duplicateLayer(size_t,  std::string);
        LightingLayer* duplicateLightingLayer(size_t, std::string);
        void moveLayer(size_t, int);
        
        void deleteLayer (const std::string&);
        size_t layerCount();
        void setLocked(size_t, bool);
        void setVisible (size_t, bool);
        void setVisible (const Renderable*, bool);
        sf::RenderStates& getRenderState (size_t);
        Layer* getLayer(std::string);
        Layer* getLayer (size_t);
        LightingLayer* getLightingLayer(std::string);
        LightingLayer* getLightingLayer (size_t);
        size_t getLayerId (std::string);
        bool isLightingLayer (size_t);
        
        void draw(sf::RenderTarget&);
    };
}

#endif
