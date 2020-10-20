#ifndef __PLATFORMERCONTROLLERCOMPONENT_HPP__
#define __PLATFORMERCONTROLLERCOMPONENT_HPP__

#include "VelEng/Component.hpp"
#include "VelEng/Updatable.hpp"
#include "VelEng/Renderable.hpp"

namespace ge{
    class PlatformerController: public Component, public Updatable, public Renderable{
    public: 
        PlatformerController();
        void onActivate() override;
        void onDeactivate() override;
        void update (sf::Time ) override;
        void handle(const ge::Event*, std::type_index, size_t) override;
        Component* copy() const override;
        std::string to_string() const;
        sf::FloatRect getGlobalBounds()const;
    private:
        void draw(sf::RenderTarget&, sf::RenderStates)const;
        bool isInAir=true;
    };
    Component* parsePlatformerController(const std::string& in);
}

#endif
