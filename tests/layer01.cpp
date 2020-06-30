#include "StaticLayer.hpp"
#include "DynamicLayer.hpp"
#include "Sprite.hpp"

using namespace ge;

int main(){
    Sprite sprites1[3] = {  
                        Sprite(sf::Color::Blue),
                        Sprite(sf::Color::Blue),
                        Sprite(sf::Color::Blue)
                         };
    Sprite sprites2[3] = {
                        Sprite(sf::Color::Green),
                        Sprite(sf::Color::Green),
                        Sprite(sf::Color::Green)
                         };
    
    sprites1[0].move(100,100);
    sprites1[0].scale(10,10);
    sprites1[1].move(250,100);
    sprites1[1].scale(5,10);
    sprites1[2].move(375,100);
    sprites1[2].scale(10,5);
    
    sprites2[0].move(110,150);
    sprites2[0].scale(10,10);
    sprites2[1].move(260,150);
    sprites2[1].scale(5,10);
    sprites2[2].move(385,150);
    sprites2[2].scale(10,5);
                       
    StaticLayer l1;
    DynamicLayer l2, l3;
    l1.addRenderable(&sprites1[0]);
    l1.addRenderable(&sprites1[1]);
    l1.addRenderable(&sprites1[2]);
    l1.display();
    
    l2.addRenderable(&sprites2[0]);
    l2.addRenderable(&sprites2[1]);
    l2.addRenderable(&sprites2[2]);
    
    sprites1[0].move(20,20);
    sprites1[1].move(20,20);
    sprites1[2].move(20,20);
    sprites1[0].setColor(sf::Color::Red);
    sprites1[1].setColor(sf::Color::Red);
    sprites1[2].setColor(sf::Color::Red);
    
    l3.addRenderable(&sprites1[0]);
    l3.addRenderable(&sprites1[1]);
    l3.addRenderable(&sprites1[2]);
    
    
    sf::RenderWindow w(sf::VideoMode(600,600), "layer01");
    
    int i = 0;
    int j = 1;
    int k = 2;
    
    while(w.isOpen()){
        sf::Event e;
        while(w.pollEvent(e)){
            switch(e.type){
                case sf::Event::Closed:
                    w.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    l1.setVisible(&sprites1[i],true);
                    l2.setVisible(&sprites2[j],true);
                    l3.setVisible(&sprites1[k],true);
                    i = (i+1)%3;
                    j = (j+1)%3;
                    k = (k+1)%3;
                    l1.setVisible(&sprites1[i],false);
                    l2.setVisible(&sprites2[j],false);
                    l3.setVisible(&sprites1[k],false);
                    break;
            }
        }
        
        
    w.clear();
    w.draw(l1);
    w.draw(l2);
    w.draw(l3);
    w.display();
    
    }
    
}
