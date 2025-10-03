#pragma once
#include <PxPhysicsAPI.h>
#include "core.hpp"

class RenderItem;
class Entity {
public:
    virtual ~Entity();
    virtual void update(double t) = 0;

    // Getters
    physx::PxTransform getTransform() const { return _transform; };
    RenderItem* getRenderItem() const { return _renderItem; };
    bool is_alive()const { return _alive; };
    void setAlive(bool alive) { _alive = alive; }

protected:
    Entity(Vector3 pos, physx::PxShape* _shape, Vector4 color);
    physx::PxTransform _transform;
    RenderItem* _renderItem;
    bool _alive;
};

