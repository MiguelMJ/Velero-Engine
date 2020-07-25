#include "LightSource.hpp"

namespace ge{
    using namespace util;
    
    bool l_firstConstructor(true);
    sf::Texture l_lightTexture;
    
    std::vector<Segment> LightSource::s_defaultSegmentPool;
    LightSource::LightSource()
        : m_polygon(sf::TriangleFan, 1)
        , m_glow(true)
        , m_color(sf::Color::White)
        , m_ptrSegmentPool(&s_defaultSegmentPool)
        {
        // The first time we call a contructor, we create the texture
        // for all LightSources
        if(l_firstConstructor){
            float maxRadius = 100.0f;
            int points = 48;
            sf::RenderTexture lightTexture;
            lightTexture.create(maxRadius*2, maxRadius*2);
            sf::VertexArray lightShape(sf::TriangleFan, points+2);
            float step = 2*3.1415/points;
            lightShape[0].position = {maxRadius,maxRadius};
            for(int i = 1; i < points+2; i++){
                lightShape[i].position = {
                    std::sin(step*(i))*maxRadius+maxRadius,
                    std::cos(step*(i))*maxRadius+maxRadius
                };
                lightShape[i].color.a = 0;
            }
            lightTexture.setSmooth(true);
            lightTexture.clear(sf::Color::Transparent);
            lightTexture.draw(lightShape);
            lightTexture.display();
            l_lightTexture = lightTexture.getTexture();
            
            l_firstConstructor = false;
        } // endif
        auto s = l_lightTexture.getSize();
        Transformable::setOrigin(s.x*0.5f, s.y*0.5f);
        m_bounds.width = s.x;
        m_bounds.height = s.y;
        setRadius(1.0f);
        castLight();
    }
    
    void LightSource::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        s.transform *= Transformable::getTransform();
        s.texture = &l_lightTexture;
        t.draw(m_polygon, s);
    }
    
    sf::FloatRect LightSource::getGlobalBounds() const{
        return Transformable::getTransform().transformRect(m_bounds);
    }
    
    void LightSource::setIntensity(float intensity){
        m_color.a = 255 * intensity;
        m_polygon[0].color = m_color;
    }
    void LightSource::setColor(sf::Color c){
        m_color.r = c.r;
        m_color.g = c.g;
        m_color.b = c.b;
        m_polygon[0].color = m_color;
    }
    void LightSource::setRadius(float r){
        r = std::max(0.f,r);
        Transformable::setScale(r,r);
    }
    void LightSource::setGlow(bool g){
        m_glow = g;
    }
    void LightSource::castLight(){
        auto bounds = ge::segmentsFromRect(Transformable::getTransform().transformRect(m_bounds));
        auto castRay = [&] (Ray r) -> sf::Vector2f {
            sf::Vector2f ret(r.origin);   
            float minRange = std::numeric_limits<float>::infinity();
            if(m_ptrSegmentPool != nullptr){
                for(auto& seg : *m_ptrSegmentPool){
                    auto t = ge::intersection(r, ge::make_ray(seg));
                    if(t.first > 0 && t.first < minRange && 
                        t.second >= 0 && t.second <= 1){
                        minRange = t.first;
                        ret = r.origin + r.direction * minRange;
                    }
                }
            }
            for(auto& seg : bounds){
                auto t = ge::intersection(r, ge::make_ray(seg));
                if(t.first > 0 && t.first < minRange && 
                    t.second >= 0 && t.second <= 1){
                    minRange = t.first;
                    ret = r.origin + r.direction * minRange;
                }
            }
            return ret;
        };
        std::vector<Ray> rays;
        int s = m_ptrSegmentPool == nullptr ? 0 : m_ptrSegmentPool->size();
        rays.reserve(4 + s * 2 * 3); // 4: corners of bounds, 2: pnts/sgmnt, 3 rays/pnt
        auto castPoint = Transformable::getPosition();
        rays.push_back(make_ray(castPoint, deg2rad(45.f)));
        rays.push_back(make_ray(castPoint, deg2rad(135.f)));
        rays.push_back(make_ray(castPoint, deg2rad(225.f)));
        rays.push_back(make_ray(castPoint, deg2rad(315.f)));
        if(m_ptrSegmentPool != nullptr){
            for(auto& s : *m_ptrSegmentPool){
                Ray r1 = make_ray(castPoint, s.first);
                Ray r2 = make_ray(castPoint, s.second);
                rays.push_back(r1);
                rays.push_back(make_ray(castPoint, r1.angle + 0.001f));
                rays.push_back(make_ray(castPoint, r1.angle - 0.001f));
                rays.push_back(r2);
                rays.push_back(make_ray(castPoint, r2.angle + 0.001f));
                rays.push_back(make_ray(castPoint, r2.angle - 0.001f));
            }
        }
        std::sort(rays.begin(), rays.end(),
                  [](Ray& r1, Ray& r2){ return r1.angle < r2.angle; });
        sf::Transform tr_i = Transformable::getTransform().getInverse();
        m_polygon.resize(rays.size() + 2);
        m_polygon[0].color = m_color;
        m_polygon[0].position = 
        m_polygon[0].texCoords = tr_i.transformPoint(castPoint);
        for(int i=0; i < rays.size(); i++){
            sf::Vector2f p = tr_i.transformPoint(castRay(rays[i]));
            m_polygon[i+1].position = p;
            m_polygon[i+1].texCoords = p;
            m_polygon[i+1].color = m_color;
        }
        m_polygon[rays.size()+1] = m_polygon[1];
    }
}
