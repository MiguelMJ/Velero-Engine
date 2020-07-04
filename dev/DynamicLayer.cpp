#include "DynamicLayer.hpp"

namespace ge{
    void DynamicLayer::draw (sf::RenderTarget& t, sf::RenderStates s) const {
        for(auto& vc: m_visibleComponents){
            t.draw(*vc, s);
        }
    }
    void DynamicLayer::addRenderable(const Renderable* r){
        m_visibleComponents.insert(r);
    }
    void DynamicLayer::removeRenderable(const Renderable* r){
        m_visibleComponents.erase(r);
        m_invisibleComponents.erase(r);
    }
    void DynamicLayer::clear(){
        m_visibleComponents.clear();
        m_invisibleComponents.clear();
    }
    void DynamicLayer::setVisible(const Renderable* r, bool v){
        if(v){
            auto it = m_invisibleComponents.find(r);
            if(it != m_invisibleComponents.end()){
                m_visibleComponents.insert(r);
                m_invisibleComponents.erase(it);
            }
        }else{
            auto it = m_visibleComponents.find(r);
            if(it != m_visibleComponents.end()){
                m_invisibleComponents.insert(r);
                m_visibleComponents.erase(it);
            }
        }
    }
}
