/**
 * @file
 * @author Miguel Mejía Jiménez
 * @copyright MIT License
 * @brief This file contains the DynamicLayer class.
 */
#ifndef __DYNAMICLAYER_HPP__
#define __DYNAMICLAYER_HPP__

#include <set>

#include "Layer.hpp"
#include "log.hpp"

namespace ge{
    /**
     * @brief %Layer that allows changes in it.
     */
    class DynamicLayer: public Layer{
    private:
        std::set<const Renderable*> m_renderables;
        void draw (sf::RenderTarget&, sf::RenderStates) const override;
        
        /**
         * @brief This friendship is neccesary in order to
         * construct a static layer from a dynamic one.
         */
        friend class StaticLayer;
    public:
        /**
         * @copydoc Layer::addRenderable
         */
        void addRenderable(const Renderable* renderable) override;
        
        /**
         * @copydoc Layer::removeRenderable
         */
        void removeRenderable(const Renderable* renderable) override;
        
        /**
         * @copydoc Layer::clear
         */
        void clear() override;
    };
}

#endif
