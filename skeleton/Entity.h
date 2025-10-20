#pragma once
#include <PxPhysicsAPI.h>
#include <memory>
#include "core.hpp"

class RenderItem;
class Entity {
public:
    virtual ~Entity();
    virtual void update(double t) = 0;

    // Getters
    physx::PxTransform getTransform() const { return _transform; };
    RenderItem* getRenderItem() const { return _renderItem.get(); };
    bool is_alive()const { return _alive; };
    void setAlive(bool alive) { _alive = alive; }
    void create_renderItem();

protected:
    Entity(Vector3 pos, physx::PxShape* shape, Vector4 color);
    physx::PxTransform _transform;
    std::unique_ptr<RenderItem> _renderItem;
    bool _alive;

private:
    physx::PxShape* _shape;
    Vector4 _color;
};

