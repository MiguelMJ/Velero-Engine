/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the class Render System.
 */
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
    /**
     * @brief System to group [layers](@ref Layer), manage and render 
     * them.
     * @details This system consists on a stack of [layers](@ref Layer) 
     * and a view, that can be thought of as the camera. By default the 
     * layers are [dynamic](@ref DynamicLayer), but can be made [static]
     * (@ref StaticLayer) by locking them. the operations that return a 
     * layer have an equivalent returning a [lighting layer]
     * (@ref LightingLayer), making a static cast without type checking. 
     * Using a dynamic layer as a lighting layer is 
     * not that dangerous, but the other way around can produce an 
     * undefined behaviour, so the user must differentiate them well.
     * 
     * For such purpose, all layers must have a name, and it is 
     * recommendable to always access and modify them using it, to make 
     * clear the nature of each layer.
     * 
     * Also, each layer of the system has its own [render states](@ref 
     * sf::RenderStates), so a transform, shader, blend mode or even 
     * texture can be applied to it.
     */
    class RenderSystem{
    private:
        std::vector<std::unique_ptr<Layer>> m_layers;
        std::map<std::string, size_t> m_layerIndex;
        std::map<size_t, std::unique_ptr<Layer>> m_lockedLayers;
        std::vector<bool> m_layerIsLocked;
        std::vector<bool> m_layerIsVisible;
        std::vector<sf::RenderStates> m_renderStates;
    public:
        /**
         * @brief View associated to the system.
         */
        sf::View m_view;
        
        /**
         * @brief Create a new layer and return a pointer to it.
         * @details The layer is created at the top of the stack.
         * @param name Name of the new layer, used to access it.
         * @returns Pointer to an empty @ref DynamicLayer.
         * @see pushLightingLayer getLayerId
         */
        Layer* pushLayer(const std::string& name);
        
        /**
         * @brief Create a new lighting layer and return a pointer to it.
         * @details The layer is created at the top of the stack.
         * @param name Name of the new layer, used to access it.
         * @returns Pointer to an empty @ref LightingLayer.
         * @see pushLayer isLightingLayer getLightingLayer
         */
        LightingLayer* pushLightingLayer(const std::string& name);
        
        /**
         * @brief Duplicate a layer and give it a name and return a 
         * pointer to it.
         * @details No matter the position of the layer to duplicate, the 
         * new one will be put on the top.
         * @param layer_id Position of the layer to duplicate, being 0 
         * the bottom of the stack.
         * @param name Name of the new layer, used to access it.
         * @returns Pointer to the duplicated layer or null by default.
         * @see duplicateLightingLayer
         */
        Layer* duplicateLayer(size_t layer_id,  const std::string& name);
        
        /**
         * @brief Duplicate a lighting layer and give it a name and 
         * return a pointer to it.
         * @details No matter the position of the layer to duplicate, the 
         * new one will be put on the top.
         * @param layer_id Position of the layer to duplicate, being 0 
         * the bottom of the stack.
         * @param name Name of the new layer, used to access it.
         * @returns Pointer to the duplicated layer or null by default.
         * @see duplicateLayer isLightingLayer
         */
        LightingLayer* duplicateLightingLayer(size_t layer_id, const std::string& name);
        
        /**
         * @brief Move a layer a number of positions in the stack.
         * @details For example, if the layers are disposed as 
         * {"A","B","C","D"} and this function is called with @p 
         * layer_id=2 and @p offset=-2, the result will be 
         * {"C","A","B","D"}.
         * @param layer_id Position of the layer to move.
         * @param offset Positive or negative number of positions to 
         * move.
         */
        void moveLayer(size_t layer_id, int offset);
        
        /**
         * @brief Delete a layer from the stack.
         * @param name Name of the layer to delete.
         */
        void deleteLayer (const std::string& name);
        
        /**
         * @brief Get the number of layers.
         * @returns The number of layers in the system.
         */
        size_t layerCount() const;
        
        /**
         * @brief Lock or unlock a layer.
         * @details A locked layer is substituted for a [static layer]
         * (@ref StaticLayer), so changes are not applied to it. The 
         * normal version of the layer is stored, in case it is unlocked.
         * @param layer_id Position of the layer to lock/unlock.
         * @param locked True to lock, false to unlock.
         */
        void setLocked(size_t layer_id, bool locked);
        
        /**
         * @brief Toggle the visibility of a layer.
         * @param layer_id Position of the layer to make in/visible.
         * @param visible True to make visible, false to make invisible.
         */
        void setVisible (size_t layer_id, bool visible);
        
        /**
         * @brief Toggle the visibilit of a layer in all the layers.
         * @param ren_ptr Pointer to the renderable to make in/visible.
         * @param visible True to make visible, false to make invisible.
         */
        void setVisible (const Renderable* ren_ptr, bool visible);
        
        /**
         * @brief Get a reference to the render states of a layer.
         * @param layer_id Position of the layer  to get the render 
         * states.
         * @returns Reference to the render states of a layer.
         */
        sf::RenderStates& getRenderState (size_t layer_id);
        
        /**
         * @brief Get a pointer to a layer, given its name.
         * @param name Name of the layer to get.
         * @returns A pointer to a layer or null by default.
         * @see getLightingLayer
         */
        Layer* getLayer(const std::string& name) const;
        
        /**
         * @brief Get a pointer to a layer, given its position.
         * @param layer_id Position of the layer to get.
         * @returns A pointer to a layer or null by default.
         * @see getLightingLayer
         */
        Layer* getLayer (size_t layer_id) const;
        
        /**
         * @brief Get a pointer to a lighting layer, given its name.
         * @param name Name of the layer to get.
         * @returns A pointer to a layer or null by default.
         * @see getLayer isLightingLayer
         */
        LightingLayer* getLightingLayer(const std::string& name) const;
        
        /**
         * @brief Get a pointer to a lighting layer, given its position.
         * @param layer_id Position of the layer to get.
         * @returns A pointer to a layer or null by default.
         * @see getLayer isLightingLayer
         */
        LightingLayer* getLightingLayer (size_t layer_id) const;
        
        /**
         * @brief Get the position of a layer given its name.
         * @param name Name of the layer to get the position of.
         * @returns Position of the layer, or -1 by default.
         */
        size_t getLayerId (const std::string& name) const;
        
        /**
         * @brief Check if a layer, given its position, is a lighting 
         * layer.
         * @param layer_id Position of the layer to check.
         * @returns True if the layer is an instance of 
         * @ref LightingLayer.
         */
        bool isLightingLayer (size_t layer_id) const;
        
        /**
         * @brief Renders the layers to the target, using separate render
         * states for each one.
         * @param target Render Target to draw the layers.
         */
        void draw(sf::RenderTarget& target);
    };
}

#endif
