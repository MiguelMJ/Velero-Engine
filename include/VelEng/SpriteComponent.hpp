#ifndef __SPRITECOMPONENT_HPP__
#define __SPRITECOMPONENT_HPP__

#include <istream>

#include "SFML/Graphics.hpp"

#include "VelEng/vertexarray.hpp"
#include "VelEng/log.hpp"
#include "VelEng/Context.hpp"
#include "VelEng/Component.hpp"

namespace ge{
    class Sprite: public Renderable, public Component{
    private:
        sf::VertexArray m_quad;
        sf::Texture* m_texture;
        std::string m_layer;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        friend Component* parseSprite(const std::string& in);
    public:
        Sprite();
        ~Sprite();
        sf::FloatRect getGlobalBounds() const override;
        void onActivate() override;
        void onDeactivate() override;
        Component* copy() const override;
        std::string to_string() const override;
        void handle(const Event *event, std::type_index type, size_t channel);
        void setLayer(const std::string& layer);
        std::string getLayer();
        Sprite& setTexture(sf::Texture* texture, sf::FloatRect rect={0,0,0,0});
        Sprite& setTextureRect(sf::FloatRect rect);
        Sprite& setColor(sf::Color color);
    };
    Component* parseSprite(const std::string& in);
}

#endif
