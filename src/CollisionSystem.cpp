#include "CollisionSystem.hpp"

namespace ge{
    // https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_PolygonCollisions1.cpp
    bool separatedAxisTheoremCollision(const Polygon& r1, const Polygon& r2, sf::Vector2f& solution){
        const Polygon *poly1 = &r1;
        const Polygon *poly2 = &r2;

        float overlap = INFINITY;
        
        for (int shape = 0; shape < 2; shape++){
            if (shape == 1){
                poly1 = &r2;
                poly2 = &r1;
            }

            for (unsigned a = 0; a < poly1->size(); a++){
                unsigned b = (a + 1) % poly1->size();
                sf::Vector2f axisProj = { -((*poly1)[b].y - (*poly1)[a].y), (*poly1)[b].x - (*poly1)[a].x };
                
                // Optional normalisation of projection axis enhances stability slightly
                //float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
                //axisProj = { axisProj.x / d, axisProj.y / d };

                // Work out min and max 1D points for r1
                float min_r1 = INFINITY, max_r1 = -INFINITY;
                for (unsigned p = 0; p < poly1->size(); p++){
                    float q = ((*poly1)[p].x * axisProj.x + (*poly1)[p].y * axisProj.y);
                    min_r1 = std::min(min_r1, q);
                    max_r1 = std::max(max_r1, q);
                }

                // Work out min and max 1D points for r2
                float min_r2 = INFINITY, max_r2 = -INFINITY;
                for (unsigned p = 0; p < poly2->size(); p++){
                    float q = ((*poly2)[p].x * axisProj.x + (*poly2)[p].y * axisProj.y);
                    min_r2 = std::min(min_r2, q);
                    max_r2 = std::max(max_r2, q);
                }

                // Calculate actual overlap along projected axis, and store the minimum
                float curroverlap = std::min(max_r1, max_r2) - std::max(min_r1, min_r2);
                if(curroverlap < overlap){
                    overlap = curroverlap;
                    solution = normalize(axisProj) * overlap;
                }
                
                if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
                    return false;
            }
        }

        // If we got here, the objects have collided, we will displace r1
        // by overlap along the vector between the two object centers
        /*sf::Vector2f d = { r2.pos.x - r1.pos.x, r2.pos.y - r1.pos.y };
        float s = sqrtf(d.x*d.x + d.y*d.y);
        r1.pos.x -= overlap * d.x / s;
        r1.pos.y -= overlap * d.y / s;*/
        return true;
    }
    void CollisionSystem::solveCollisions(){
        for(auto& c: m_colliders){
            c->m_transformedPolygon = c->m_basePolygon;
            transform(c->m_transformedPolygon, c->getEntityPtr()->getTransform());
        }
        // LOG_F(INFO,"Solving for {} colliders!", m_colliders.size());
        for(auto it1 = m_colliders.begin(); it1 != m_colliders.end(); it1++){
            auto it2 = it1;
            it2++;
            while(it2 != m_colliders.end()){
                auto& c1 = *it1;
                auto& c2 = *it2;
                sf::Vector2f solution;
                if(separatedAxisTheoremCollision(
                    c1->m_transformedPolygon, 
                    c2->m_transformedPolygon,
                    solution)
                ){
                    int p1 = c1->m_priority, p2 = c2->m_priority;
                    if(p1 < p2){
                        c1->getEntityPtr()->move(solution);
                    }else if(p2 < p1){
                        c2->getEntityPtr()->move(-solution);
                    }else{
                        c1->getEntityPtr()->move(solution * 0.5f);
                        c2->getEntityPtr()->move(solution * 0.5f);
                    }
                }
                it2++;
            }
        }
    }
    void CollisionSystem::addCollider(Collider* collider){
        m_colliders.insert(collider);
    }
    void CollisionSystem::removeCollider(Collider* collider){
        m_colliders.erase(collider);
    }
    void CollisionSystem::clear(){
        m_colliders.clear();
    }
}
