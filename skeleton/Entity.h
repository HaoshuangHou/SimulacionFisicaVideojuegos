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
    Vector3 getPos() const { return _transform.p; };
    RenderItem* getRenderItem() const { return _renderItem.get(); };
    bool is_alive()const { return _alive; };
    void setAlive(bool alive) { _alive = alive; }
    void create_renderItem();
    void deregisterRenderItem();
    bool isRenderItemValid();

protected:
    Entity(Vector3 pos, physx::PxShape* shape, Vector4 color);
    Entity(Vector3 pos, physx::PxShape* shape, Vector4 color, bool renderItem);
    physx::PxTransform _transform;
    std::unique_ptr<RenderItem> _renderItem;
    bool _alive;
    physx::PxShape* _shape;
    Vector4 _color;

private:
    bool _renderItemRegistered;
};

