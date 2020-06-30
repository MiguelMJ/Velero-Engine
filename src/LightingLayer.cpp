#include "LightingLayer.hpp"

namespace ge{
    
    sf::BlendMode l_lightBlend(
        sf::BlendMode::Factor::Zero,              // color src
        sf::BlendMode::Factor::One,          // color dst
        sf::BlendMode::Equation::Add,             // color eq
        sf::BlendMode::Factor::Zero,              // alpha src
        sf::BlendMode::Factor::OneMinusSrcAlpha,  // alpha dst
        sf::BlendMode::Equation::Add);            // alpha eq
    sf::BlendMode l_glowBlend = sf::BlendAdd;
    
    LightingLayer::LightingLayer()
    : m_fogColor(sf::Color::Black)
    , m_fogQuad(sf::Quads, 4)
    {
        setFogSize(10,10);
    }
    void  LightingLayer::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        s.texture = &m_fogTexture.getTexture();
        t.draw(m_fogQuad, s);
        s.blendMode = l_glowBlend;
        for(auto& ls: m_visibleLights){
            if(ls->m_glow){
                t.draw(*ls, s);
            }
        }
    }
    void LightingLayer::addRenderable(const Renderable* r) {
        const LightSource* ls = dynamic_cast<const LightSource*> (r);
        const_cast<LightSource*>(ls) -> m_ptrSegmentPool = &m_segmentPool;;
        m_visibleLights.insert(ls);
    }
    void LightingLayer::setVisible(const Renderable* r, bool v){
        const LightSource* ls = dynamic_cast<const LightSource*> (r);
        if(v){
            auto it = m_invisibleLights.find(ls);
            if(it != m_invisibleLights.end()){
                m_visibleLights.insert(ls);
                m_invisibleLights.erase(it);
            }
        }else{
            auto it = m_visibleLights.find(ls);
            if(it != m_visibleLights.end()){
                m_invisibleLights.insert(ls);
                m_visibleLights.erase(it);
            }
            
        }
    }
    void LightingLayer::setFogSize(float x, float y){
        m_fogTexture.create(x,y);
        m_fogTexture.setSmooth(true);
        m_fogQuad[0].position =
        m_fogQuad[0].texCoords = {0,0};
        m_fogQuad[1].position =
        m_fogQuad[1].texCoords = {x,0};
        m_fogQuad[2].position =
        m_fogQuad[2].texCoords = {x,y};
        m_fogQuad[3].position =
        m_fogQuad[3].texCoords = {0,y};
        ge::move(m_fogQuad, m_fogOffset);
    }
    void LightingLayer::setFogPosition(float x, float y){
        ge::move(m_fogQuad, -m_fogOffset);
        m_fogOffset = {x,y};
        ge::move(m_fogQuad, m_fogOffset);
    }
    void LightingLayer::setFogColor(sf::Color c){
        m_fogColor.r = c.r;
        m_fogColor.g = c.g;
        m_fogColor.b = c.b;
    }
    void LightingLayer::setFogOpacity(float o){
        m_fogColor.a = 255 * o;
    }
    void LightingLayer::updateFog(){
        m_fogTexture.clear(m_fogColor);
        sf::RenderStates fogrs;
        fogrs.blendMode = l_lightBlend;
        fogrs.transform.translate(-m_fogOffset);
        for(auto& ls: m_visibleLights){
            m_fogTexture.draw(*ls, fogrs);
        }
        m_fogTexture.display();
    }
}
