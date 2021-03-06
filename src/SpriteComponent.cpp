#include "VelEng/SpriteComponent.hpp"
#include "VelEng/Serialize/Parser.hpp"
namespace ven{
    
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
            ven::setTextureRect(m_quad, r);
        }else{
            ven::setTextureRect(m_quad, rect);
        }
        return *this;
    }
    Sprite& Sprite::setTextureRect(sf::FloatRect rect){
        auto r2 = rect;
        r2.top=  r2.left = 0;
        m_quad = getQuad(r2);
        ven::setTextureRect(m_quad, rect);
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
    Component* parseSprite(const JSON& json){
        auto ret = new Sprite();
        std::string texture = DOMget<std::string>(json, "texture", "");
        AssetSystem::load(texture);
        auto tptr = AssetSystem::getTexture(texture);
        ret -> setTexture(tptr);
        auto ts = tptr->getSize();
        ret -> m_layer = DOMget<std::string>(json, "layer");
        float x = DOMget<float>(json,"x",0);
        float y = DOMget<float>(json,"y",0);
        ret -> move(x,y);
        float w = DOMget<float>(json,"width",DOMget<float>(json,"w",ts.x));
        float h = DOMget<float>(json,"height",DOMget<float>(json,"h",ts.y));
        float tx = DOMget<float>(json,"tx",0);
        float ty = DOMget<float>(json,"ty",0);
        float tw = DOMget<float>(json,"twidth",DOMget<float>(json,"tw",ts.x));
        float th = DOMget<float>(json,"theight",DOMget<float>(json,"th",ts.y));
        ret -> setTextureRect(sf::FloatRect(tx,ty,tw,th));
        auto gb = ret->getGlobalBounds();
        ret -> scale(w/gb.width, h/gb.height);
        return ret;
    }
}
