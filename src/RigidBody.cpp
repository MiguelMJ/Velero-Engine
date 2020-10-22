#include "VelEng/RigidBody.hpp"

#include "fmt/core.h"

#include "VelEng/types.hpp"
#include "VelEng/vertexarray.hpp"
#include "VelEng/Context.hpp"

namespace ven{
    RigidBody::RigidBody(){

    }
    Component* RigidBody::copy() const{
        return nullptr;
    }
    void RigidBody::update(sf::Time){
        auto pos = m_body->GetPosition();
        auto rot = m_body->GetAngle();
        getEntityPtr()->setPosition(pos.x, pos.y);
        getEntityPtr()->setRotation(rot);
    }
    void RigidBody::onAdd(){
        m_body = M_PS.CreateBody(&m_bodyDef);
        for(auto& f : m_fixtureDefs){
            m_body->CreateFixture(&f);
        }
    }
    void RigidBody::onRemove(){
        M_PS.DestroyBody(m_body);
    }
    void RigidBody::onActivate(){
        M_RSD.getLayer("debug")->addRenderable(this);
        m_body->SetEnabled(true);
    }
    void RigidBody::onDeactivate(){
        M_RSD.getLayer("debug")->removeRenderable(this);
        m_body->SetEnabled(false);
    }
    std::string RigidBody::to_string() const{
        return "NOT IMPLEMENTED";
    }
    void RigidBody::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        s.transform *= getEntityPtr()->getTransform();
        t.draw(B2toSFML(*((b2PolygonShape*)m_fixtureDefs[0].shape)), s);
    }
    sf::FloatRect RigidBody::getGlobalBounds() const{
        // return getLines(m_transformedPolygon).getBounds();
        return sf::FloatRect();
    }
    Component* parseRigidBody(const std::string& ){
        /*auto ret = new RigidBody;
        ret->m_bodyDef.enabled = false;
        auto dict = parseMap(line);
        for(auto& kv : dict){
            if(kv.first == "polygon"){
                b2PolygonShape poly = parseB2Polygon(kv.second);
                b2FixtureDef fdef;
                fdef.shape = &poly;
                ret->m_fixtureDefs.push_back(fdef);
            }else if(kv.first == "type"){
                if(kv.second == "static"){
                    ret->m_bodyDef.type = b2_staticBody;
                }else if(kv.second == "kinematic"){
                    ret->m_bodyDef.type = b2_kinematicBody;
                }else if(kv.second == "dynamic"){
                    ret->m_bodyDef.type = b2_dynamicBody;
                }else{
                    LOG_F(WARNING, "Unrecognized value for key type: {}", kv.second);
                }
            }else if(kv.first == "gravityScale"){
                ret->m_bodyDef.gravityScale = stof(kv.second);
            }else if(kv.first == "linearVelocity"){
                ret->m_bodyDef.linearVelocity = parseB2Vector(kv.second);
            }else if(kv.first == "angularVelocity"){
                ret->m_bodyDef.angularVelocity = std::stof(kv.second);
            }else if(kv.first == "linearDamping"){
                ret->m_bodyDef.linearDamping = std::stof(kv.second);
            }else if(kv.first == "angularDamping"){
                ret->m_bodyDef.angularDamping = std::stof(kv.second);
            }else if(kv.first == "fixedRotation"){
                ret->m_bodyDef.fixedRotation = kv.second == "on";
            }else{
                LOG_F(WARNING, "Unrecognized key for Collider: {}", kv.first);
            }
        }
        return ret;*/
        return nullptr;
    }
}
