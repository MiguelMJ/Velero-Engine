#include "StaticLayer.hpp"

namespace ge{
    StaticLayer::StaticLayer()
    : m_quad(sf::Quads, 4)
    {
        
    }
    StaticLayer::StaticLayer(const StaticLayer& sl)
    : m_quad(sl.m_quad)
    , m_offset(sl.m_offset)
    {
        auto s = sl.m_renderTexture.getSize();
        m_renderTexture.create(s.x, s.y);
        sf::RenderStates rs;
        rs.transform.translate(-m_offset);
        m_renderTexture.draw(sl, rs);
        m_renderTexture.display();
    }
    StaticLayer::StaticLayer(const DynamicLayer& dl):
    StaticLayer()
    {
        for(auto& r: dl.m_visibleComponents){
            addRenderable(r);
        }
        display();
    }
    StaticLayer::StaticLayer(const LightingLayer& ll):
    StaticLayer()
    {
        sf::Vector2f limit;
        m_offset = {
            std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::infinity()
        };
        if(ll.m_fogColor.a > 0){
            m_offset = ll.m_fogOffset;
            auto s = ll.m_fogTexture.getSize();
            limit.x = m_offset.x + s.x;
            limit.y = m_offset.y + s.y;
        }
        for(auto& r: ll.m_visibleLights){
            auto b = r->getGlobalBounds();
            limit = {
                std::max(limit.x, b.left + b.width),
                std::max(limit.y, b.top + b.height)
            };
            m_offset = {
                std::min(m_offset.x, b.left),
                std::min(m_offset.y, b.top)
            };
        }
        
        sf::Vector2f size = limit - m_offset;
        m_quad[0].position =
        m_quad[0].texCoords = {0, size.y};
        m_quad[1].position =
        m_quad[1].texCoords = {0, 0};
        m_quad[2].position =
        m_quad[2].texCoords = {size.x, 0};
        m_quad[3].position =
        m_quad[3].texCoords = {size.x, size.y};
        
        m_renderTexture.create(size.x, size.y);
        m_renderTexture.clear(sf::Color::Transparent);
        sf::RenderStates st;
        st.transform.translate(-m_offset);
        m_renderTexture.draw(ll, st);
        m_renderTexture.display();
    }
    void StaticLayer::draw(sf::RenderTarget& t, sf::RenderStates s)const{
        s.transform.translate(m_offset);
        s.texture = &m_renderTexture.getTexture();
        t.draw(m_quad, s);
    }
    void StaticLayer::addRenderable(const Renderable* r){
        m_unrendered.push_back(r);
    }
    void StaticLayer::removeRenderable(const Renderable*){}
    void StaticLayer::clear(){
        m_renderTexture.clear(sf::Color::Transparent);
    }
    void StaticLayer::setVisible(const Renderable*,bool){}
    void StaticLayer::display(){
        if(m_unrendered.size() > 0){
            auto b = m_unrendered[0]->getGlobalBounds();
            sf::Vector2f limit = {
                b.left + b.width, 
                b.top + b.height
            };
            m_offset = {b.left, b.top};
            for(auto& r: m_unrendered){
                b = r->getGlobalBounds();
                limit = {
                    std::max(limit.x, b.left + b.width),
                    std::max(limit.y, b.top + b.height)
                };
                m_offset = {
                    std::min(m_offset.x, b.left),
                    std::min(m_offset.y, b.top)
                };
            }
            sf::Vector2f size = limit - m_offset;
            m_quad[0].position =
            m_quad[0].texCoords = {0, size.y};
            m_quad[1].position =
            m_quad[1].texCoords = {0, 0};
            m_quad[2].position =
            m_quad[2].texCoords = {size.x, 0};
            m_quad[3].position =
            m_quad[3].texCoords = {size.x, size.y};
            
            m_renderTexture.create(size.x, size.y);
            m_renderTexture.clear(sf::Color::Transparent);
            sf::RenderStates st;
            st.transform.translate(-m_offset);
            for(auto& r: m_unrendered){
                m_renderTexture.draw(*r, st);
            }
            m_renderTexture.display();
            m_unrendered.clear();
        }
    }
}
