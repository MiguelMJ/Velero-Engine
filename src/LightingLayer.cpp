#include "LightingLayer.hpp"

namespace ge{

    sf::BlendMode l_lightBlend(
        sf::BlendMode::Factor::Zero,              // color src
        sf::BlendMode::Factor::One,               // color dst
        sf::BlendMode::Equation::Add,             // color eq
        sf::BlendMode::Factor::Zero,              // alpha src
        sf::BlendMode::Factor::OneMinusSrcAlpha,  // alpha dst
        sf::BlendMode::Equation::Add);            // alpha eq
    sf::BlendMode l_glowBlend = sf::BlendAdd;
    
    LightingLayer::LightingLayer()
    : m_fogQuad(sf::Quads, 4)
    , m_fogColor(sf::Color::Black)
    {
        setFogSize(10,10);
    }
    LightingLayer::LightingLayer(const LightingLayer& ll)
    : m_lights(ll.m_lights)
    , m_fogQuad(ll.m_fogQuad)
    , m_fogOffset(ll.m_fogOffset)
    , m_fogColor(ll.m_fogColor)
    {
        auto s = m_fogTexture.getSize();
        m_fogTexture.create(s.x, s.y);
        updateFog();
    }
    void  LightingLayer::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        s.texture = &m_fogTexture.getTexture();
        t.draw(m_fogQuad, s);
        s.blendMode = l_glowBlend;
        for(auto& ls: m_lights){
            if(ls->m_glow){
                t.draw(*ls, s);
            }
        }
    }
    void LightingLayer::addRenderable(const Renderable* r) {
        const LightSource* ls = static_cast<const LightSource*> (r);
        const_cast<LightSource*>(ls) -> m_ptrSegmentPool = &m_segmentPool;;
        m_lights.insert(ls);
    }
    void LightingLayer::removeRenderable(const Renderable* r){
        auto ls = static_cast<const LightSource*>(r);
        m_lights.erase(ls);
    }
    void LightingLayer::clear(){
        m_lights.clear();
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
    void LightingLayer::adjustFog(const sf::View& view){
        auto size = view.getSize();
        auto center = view.getCenter();
        auto position = center - size/2.0f;
        setFogPosition(position.x, position.y);
        setFogSize(size.x, size.y);
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
        if(m_fogColor.a > 0){
            m_fogTexture.clear(m_fogColor);
            sf::RenderStates fogrs;
            fogrs.blendMode = l_lightBlend;
            fogrs.transform.translate(-m_fogOffset);
            for(auto& ls: m_lights){
                m_fogTexture.draw(*ls, fogrs);
            }
            m_fogTexture.display();
        }
    }
}
