#include "VelEng/RigidBody.hpp"

#include "fmt/core.h"

#include "VelEng/types.hpp"
#include "VelEng/vertexarray.hpp"
#include "VelEng/Context.hpp"

namespace ven{
    std::vector<b2FixtureDef> RigidBody::s_fixtureDefs;
    RigidBody::RigidBody(){

    }
    Component* RigidBody::copy() const{
        auto ret = new RigidBody();
        ret->m_fixtureDefs = m_fixtureDefs;
        ret->m_bodyDef = m_bodyDef;
        return ret;
    }
    void RigidBody::update(sf::Time){
        auto pos = m_body->GetPosition();
        auto rot = m_body->GetAngle();
        b2MassData md;
        m_body->GetMassData(&md);
        getEntityPtr()->setPosition(pos.x, -pos.y);
        // auto ori = getEntityPtr()->getOrigin();
        //getEntityPtr()->setOrigin(B2toSFML(md.center));
        getEntityPtr()->setRotation(rad2deg(-rot));
        // getEntityPtr()->setOrigin(ori);
    }
    void RigidBody::onAdd(){
        m_body = M_PS.CreateBody(&m_bodyDef);
        for(auto f : m_fixtureDefs){
            m_body->CreateFixture(&s_fixtureDefs[f]);
        }
    }
    void RigidBody::onRemove(){
        M_PS.DestroyBody(m_body);
    }
    void RigidBody::onActivate(){
        M_RSD.getLayer("debug")->addRenderable(this);
        m_body->SetTransform(SFMLtoB2(getEntityPtr()->getPosition()), m_body->GetAngle());
        m_body->SetEnabled(true);
        M_LTS.addUpdatable(this);
    }
    void RigidBody::onDeactivate(){
        M_RSD.getLayer("debug")->removeRenderable(this);
        m_body->SetEnabled(false);
        M_LTS.removeUpdatable(this);
    }
    std::string RigidBody::to_string() const{
        return "RigidBody";
    }
    void RigidBody::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        s.transform *= getEntityPtr()->getTransform();
        for(auto f : m_fixtureDefs){
            t.draw(B2toSFML(*((b2PolygonShape*)s_fixtureDefs[f].shape)), s);
        }
    }
    sf::FloatRect RigidBody::getGlobalBounds() const{
        // return getLines(m_transformedPolygon).getBounds();
        return sf::FloatRect();
    }
    Component* parseRigidBody(const JSON& json){
        auto ret = new RigidBody();
        ret->m_bodyDef.enabled = false;
        std::string type = DOMget<std::string>(json,"type","dynamic");
        ret->m_bodyDef.gravityScale = DOMget<float>(json,"gravityScale",DOMget<float>(json,"gs",1));
        ret->m_bodyDef.linearVelocity = DOMget<b2Vec2>(json,"lv",DOMget<b2Vec2>(json,"linearVelocity",b2Vec2()));
        ret->m_bodyDef.angularVelocity = DOMget<float>(json,"av",DOMget<float>(json,"angularVelocity",0));
        ret->m_bodyDef.linearDamping = DOMget<float>(json,"ld",DOMget<float>(json,"linearDamping",0));
        ret->m_bodyDef.angularDamping = DOMget<float>(json,"ad",DOMget<float>(json,"angularDamping",0));
        ret->m_bodyDef.fixedRotation = DOMget<bool>(json,"fr",DOMget<bool>(json,"fixedRotation",false));
        
        b2FixtureDef fdef;
        fdef.density = 1;
        if(type == "dynamic"){
            ret->m_bodyDef.type = b2_dynamicBody;
        }else if(type == "static"){
            ret->m_bodyDef.type = b2_staticBody;
        }else if(type == "kinematic"){
            ret->m_bodyDef.type = b2_kinematicBody;
        }else{
            ABORT_F("Unrecognized body type");
        }
        
        fdef.shape = new b2PolygonShape(DOMget<b2PolygonShape>(json,"polygon"));
        ret->m_fixtureDefs.push_back(RigidBody::s_fixtureDefs.size());
        RigidBody::s_fixtureDefs.push_back(fdef);
        return ret;
    }
}
