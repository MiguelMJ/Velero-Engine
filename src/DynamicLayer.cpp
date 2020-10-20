#include "VelEng/DynamicLayer.hpp"

namespace ven{
    void DynamicLayer::draw (sf::RenderTarget& t, sf::RenderStates s) const {
        for(auto& vc: m_renderables){
            t.draw(*vc, s);
        }
    }
    void DynamicLayer::addRenderable(const Renderable* r){
        DLOG_F(INFO, "Renderable added to Dlayer");
        m_renderables.insert(r);
    }
    void DynamicLayer::removeRenderable(const Renderable* r){
        DLOG_F(INFO, "Renderable removed from Dlayer");
        m_renderables.erase(r);
    }
    void DynamicLayer::clear(){
        DLOG_F(INFO, "Dlayer cleared");
        m_renderables.clear();
    }
}
