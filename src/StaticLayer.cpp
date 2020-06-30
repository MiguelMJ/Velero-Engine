#include "StaticLayer.hpp"

namespace ge{
    StaticLayer::StaticLayer()
    : m_quad(sf::Quads, 4)
    {
        
    }
    void StaticLayer::draw(sf::RenderTarget& t, sf::RenderStates s)const{
        s.transform.translate(m_offset);
        s.texture = &m_renderTexture.getTexture();
        t.draw(m_quad, s);
    }
    void StaticLayer::addRenderable(const Renderable* r){
        m_unrendered.push_back(r);
    }
    void StaticLayer::setVisible(const Renderable*,bool){}
    void StaticLayer::display(){
        if(m_unrendered.size() > 0){
            auto b = m_unrendered[0]->getGlobalBounds();
            sf::Vector2f size = {b.width, b.height};
            m_offset = {b.left, b.top};
            for(auto& r: m_unrendered){
                b = r->getGlobalBounds();
                size = {
                    std::max(size.x, b.left + b.width),
                    std::max(size.y, b.top + b.height)
                };
                m_offset = {
                    std::min(m_offset.x, b.left),
                    std::min(m_offset.y, b.top)
                };
            }
            m_quad[0].position =
            m_quad[0].texCoords = {0, size.y};
            m_quad[1].position =
            m_quad[1].texCoords = {0, 0};
            m_quad[2].position =
            m_quad[2].texCoords = {size.x, 0};
            m_quad[3].position =
            m_quad[3].texCoords = {size.x, size.y};
            
            m_renderTexture.create(size.x, size.y);
            sf::RenderStates st;
            st.transform.translate(-m_offset);
            for(auto& r: m_unrendered){
                m_renderTexture.draw(*r, st);
            }
        }
    }
}
