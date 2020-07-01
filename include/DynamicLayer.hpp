#ifndef __DYNAMICLAYER_HPP__
#define __DYNAMICLAYER_HPP__

#include <set>

#include "Layer.hpp"

namespace ge{
    class DynamicLayer: public Layer{
    private:
        std::set<const Renderable*> m_visibleComponents;
        std::set<const Renderable*> m_invisibleComponents;
        void draw (sf::RenderTarget&, sf::RenderStates) const override;
        friend class StaticLayer;
    public:
        void addRenderable(const Renderable*) override;
        void setVisible(const Renderable*, bool) override;
    };
}

#endif
