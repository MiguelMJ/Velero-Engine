#include "LightSource.hpp"

using namespace ge;

int main(){
    
    sf::RenderWindow w(sf::VideoMode(600,600), "renderable01");
    LightSource l, m;
    float r = 1;
    l.setRadius(r);
    m.setRadius(r*1.3f);
    m.setIntensity(0.6);
    m.setColor(sf::Color::Cyan);
    w.setFramerateLimit(45);
    
    std::vector<Segment> segments;
    
    segments.push_back(Segment({100,100},{110,110}));
    segments.push_back(Segment({130,130},{120,120}));
    segments.push_back(Segment({150,150},{140,140}));
    segments.push_back(Segment({170,170},{160,160}));
    
    segments.push_back(Segment({300,300},{310,310}));
    segments.push_back(Segment({330,330},{320,320}));
    segments.push_back(Segment({350,350},{340,340}));
    segments.push_back(Segment({370,370},{360,360}));
    
    l.m_ptrSegmentPool = &segments;
    m.m_ptrSegmentPool = &segments;
    
    sf::Clock clock;
    
    while(w.isOpen()){
        sf::Time dt = clock.getElapsedTime();
        sf::Event e;
        while(w.pollEvent(e)){
            switch(e.type){
                case sf::Event::Closed:
                    w.close();
                    break;
                case sf::Event::MouseWheelMoved:
                    r += e.mouseWheel.delta/10.f;
                    lll(r);
                    l.setRadius(r);
                    break;
            }
        }
        
        l.setPosition(w.mapPixelToCoords(sf::Mouse::getPosition(w)));
        m.setPosition(sf::Vector2f(220 + 20*(std::sin(dt.asSeconds()*10)),230 + 20*(std::cos(dt.asSeconds()*10))));
        
        l.castLight();
        m.castLight();
        
        w.clear();
        
        w.draw(l);
        w.draw(m);
        
        w.display();
    }
    
    return 0;
}
