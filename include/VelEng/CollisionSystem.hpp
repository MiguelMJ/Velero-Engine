#ifndef _GE_COLLISION_SYSTEM_HPP_
#define _GE_COLLISION_SYSTEM_HPP_

#include <set>

#include "VelEng/ColliderComponent.hpp"

namespace ven{
    class CollisionSystem{
    private:
        std::set<Collider*> m_colliders;
    public:
        void solveCollisions();
        void addCollider(Collider* collider);
        void removeCollider(Collider* collider);
        void clear();
    };
}

#endif
