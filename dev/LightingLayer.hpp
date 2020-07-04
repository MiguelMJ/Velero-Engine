#ifndef __LIGHTINGLAYER_HPP__
#define __LIGHTINGLAYER_HPP__

#include <SFML/Graphics.hpp>

#include <set>

#include "Layer.hpp"
#include "LightSource.hpp"

namespace ge{
    class LightingLayer: public Layer{
    private:
        std::set <const LightSource*> m_visibleLights;
        std::set <const LightSource*> m_invisibleLights;
        sf::RenderTexture m_fogTexture;
        sf::VertexArray m_fogQuad;
        sf::Vector2f m_fogOffset;
        sf::Color m_fogColor;
        // sf::Vector2f m_fog;
        void draw(sf::RenderTarget&, sf::RenderStates)const override;
        friend class StaticLayer;
    public:
        std::vector<Segment> m_segmentPool;
        LightingLayer();
        LightingLayer(const LightingLayer&);
        void addRenderable(const Renderable*) override;
        void removeRenderable(const Renderable*) override;
        void clear() override;
        void setVisible(const Renderable*, bool) override;
        void setFogSize(float, float);
        void setFogPosition(float, float);
        void setFogColor(sf::Color);
        void setFogOpacity(float);
        void updateFog();
    };
}

#endif
