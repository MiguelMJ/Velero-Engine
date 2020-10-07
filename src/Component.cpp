#include "Component.hpp"

namespace ge{
    void Component::setActive(bool a){
        if(a && !m_active){
            m_active = true;
            onActivate();
        }else if(!a && m_active){
            m_active = false;
            onDeactivate();
        }
    }
    void Component::onAdd(){}
    void Component::onRemove(){}
    void Component::onActivate(){}
    void Component::onDeactivate(){}
    void Component::handle(const Event*,std::type_index,size_t){}
    Component::~Component(){}
    Entity* Component::getEntityPtr() const{
        return m_ptrEntity;
    }
}
