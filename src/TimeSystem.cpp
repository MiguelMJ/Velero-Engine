#include "VelEng/TimeSystem.hpp"

namespace ven{
    void TimeSystem::addUpdatable(Updatable* updatable){
        m_updatables.insert(updatable);
    }
    void TimeSystem::removeUpdatable(Updatable* updatable){
        m_updatables.erase(updatable);
    }
    void TimeSystem::update(sf::Time delta){
        delta *= m_factor;
        for(auto& u: m_updatables){
            u->update(delta);
        }
    }
}
