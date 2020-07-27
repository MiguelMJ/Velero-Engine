#include "SpriteComponent.hpp"

namespace ge{
    
    Sprite::Sprite(){
        m_quad.resize(4);
    }
    Sprite::~Sprite(){}
    void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.texture = m_texture;
        states.transform *= this->getTransform();
        states.transform *= m_ptrEntity->getTransform();
        target.draw(m_quad, states);
    }
    sf::FloatRect Sprite::getGlobalBounds() const {
        return getTransform().transformRect(m_quad.getBounds());
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
        auto r2 = rect;
        r2.top=  r2.left = 0;
        m_quad = getQuad(r2);
        ge::setTextureRect(m_quad, rect);
        return *this;
    }
    Component* Sprite::copy() const{
        return new Sprite(*this);
    }
    Component* parseSprite(std::istream& in){
        auto ret = new Sprite();
        std::string line;
        getline(in, line);
        auto dict = parseMap(line);
        bool fixedrect = false;
        bool fixedsize = false;
        sf::Vector2f ns; // new fixed size
        sf::FloatRect trec; // new fixed size
        for(auto& kv : dict){
            if(kv.first == "texture"){
                AssetSystem::load(kv.second);
                auto tptr = AssetSystem::getTexture(kv.second);
                ret->setTexture(tptr);
                if(fixedsize && tptr){
                    auto gb = tptr->getSize();
                    ret->scale(ns.x/gb.x, ns.y/gb.y);
                }
            }else if(kv.first == "pos"){
                ret->move(parseVector2<float>(kv.second));
            }else if(kv.first == "layer"){
                ret->m_layer = kv.second;
            }else if(kv.first == "size"){
                fixedsize = true;
                ns = parseVector2<float>(kv.second);
            }else if(kv.first == "rect"){
                fixedrect = true;
                trec = parseRect<float>(kv.second);
            }else{
                LOG_F(WARNING, "Unrecognized key for Sprite: {}", kv.first);
            }
        }
        if(fixedrect){
                ret->setTextureRect(trec);
        }
        if(fixedsize){
            auto gb = ret->getGlobalBounds();
            ret->scale(ns.x/gb.width, ns.y/gb.height);
        }
        return ret;
    }
}
