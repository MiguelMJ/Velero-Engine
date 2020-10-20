#include "VelEng/SpriteComponent.hpp"

namespace ge{
    
    Sprite::Sprite(){
        m_quad.resize(4);
    }
    Sprite::~Sprite(){}
    void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.texture = m_texture;
        states.transform *= m_ptrEntity->getTransform();
        states.transform *= this->getTransform();
        target.draw(m_quad, states);
    }
    sf::FloatRect Sprite::getGlobalBounds() const {
        return getTransform().transformRect(m_quad.getBounds());
    }
    void Sprite::onActivate() {
        auto l = M_RS.getLayer(m_layer);
        if (l) l->addRenderable(this);
    }
    void Sprite::onDeactivate() {
        auto l = M_RS.getLayer(m_layer);
        if (l) l->removeRenderable(this);
    }
    void Sprite::handle(const Event*, std::type_index, size_t){
        
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
    std::string Sprite::to_string() const{
        sf::Vector2f tl_tex, br_tex, tex_s;
        sf::Vector2f tl_pos, br_pos, size;
        sf::Vector2f pos;
        tl_tex = m_quad[0].texCoords;
        br_tex = m_quad[2].texCoords;
        tex_s = br_tex - tl_tex;
        tl_pos = m_quad[0].position;
        br_pos = m_quad[2].position;
        size = br_pos - tl_pos;
        pos = getPosition();
        const char* format = 
        "sprite pos = {{{}, {}}}; layer = {}; size = {{{}, {}}}; rect = {{{}, {}, {}, {}}}";
        return fmt::format(format,
                           pos.x,
                           pos.y,
                           m_layer,
                           size.x,
                           size.y,
                           tl_tex.x,
                           tl_tex.y,
                           tex_s.x,
                           tex_s.y
                    );
    }
    Component* parseSprite(const std::string& line){
        auto ret = new Sprite();
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
