#ifndef __SPRITECOMPONENT_HPP__
#define __SPRITECOMPONENT_HPP__

#include "SFML/Graphics.hpp"

#include "vertexarray.hpp"
#include "log.hpp"
#include "Context.hpp"
#include "EntityComponent.hpp"

namespace ge{
    class Sprite: public Renderable, public Component{
    private:
        sf::VertexArray m_quad;
        sf::Texture* m_texture;
        std::string m_layer;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    public:
        sf::FloatRect getGlobalBounds() const override;
        void onAdd() override;
        void onRemove() override;
        void onActivate() override;
        void onDeactivate() override;
        void handle(const Event *event, std::type_index type, size_t channel);
        void setLayer(const std::string& layer);
        std::string getLayer();
        Sprite& setTexture(sf::Texture* texture, sf::FloatRect rect);
        Sprite& setTextureRect(sf::FloatRect rect);
        Sprite& setColor(sf::Color color);
    };
}

#endif
