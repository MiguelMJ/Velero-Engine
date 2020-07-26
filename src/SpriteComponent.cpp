#include "SpriteComponent.hpp"

namespace ge{
    
    void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.texture = m_texture;
        states.transform *= this->getTransform();
        states.transform *= m_ptrEntity->getTransform();
        target.draw(m_quad, states);
    }
    sf::FloatRect Sprite::getGlobalBounds() const {
        return m_quad.getBounds();
    }
    void Sprite::onAdd(){
        auto l = M_RS.getLayer(m_layer);
        if(l != nullptr){
            l->addRenderable(this);
        }else{
            LOG_F(WARNING, "Layer {} doesn't exist", m_layer);
        }
    }
    void Sprite::onRemove() {
        auto l = M_RS.getLayer(m_layer);
        if(l != nullptr){
            l->removeRenderable(this);
        }else{
            LOG_F(WARNING, "Layer {} doesn't exist", m_layer);
        }
    }
    void Sprite::onActivate() {
        M_RS.getLayer(m_layer)->setVisible(this, true);
    }
    void Sprite::onDeactivate() {
        M_RS.getLayer(m_layer)->setVisible(this, false);
    }
    void Sprite::handle(const Event *event, std::type_index type, size_t channel){
        
    }
    void Sprite::setLayer(const std::string& layer){
        m_layer = layer;
    }
    std::string Sprite::getLayer(){
        return m_layer;
    }
    Sprite& Sprite::setTexture(sf::Texture* texture, sf::FloatRect rect){
        m_texture = texture;
        auto s = texture->getSize();
        sf::FloatRect r(0,0,s.x,s.y);
        m_quad = getQuad(r);
        if(rect.width == 0 && rect.height == 0){
            ge::setTextureRect(m_quad, r);
        }else{
            ge::setTextureRect(m_quad, rect);
        }
        return *this;
    }
    Sprite& Sprite::setTextureRect(sf::FloatRect rect){
        ge::setTextureRect(m_quad, rect);
        return *this;
    }
}
