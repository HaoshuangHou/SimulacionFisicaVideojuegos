#pragma once
#include <PxPhysicsAPI.h>
#include <memory>
#include "core.hpp"

class RenderItem;
class Entity {
public:
    Entity(const Vector3& pos, physx::PxShape* shape, Vector4 color);
    Entity(const Vector3& pos, physx::PxShape* shape, Vector4 color, bool renderItem);

    virtual ~Entity();
    virtual void update(double t) {};

    // Getters
    physx::PxTransform getTransform() const { return _transform; };
    Vector3 getPos() const { return _transform.p; };
    RenderItem* getRenderItem() const { return _renderItem.get(); };
    bool is_alive()const { return _alive; };
    void setAlive(bool alive) { _alive = alive; }
    virtual void create_renderItem();
    virtual void deregisterRenderItem();
    bool isRenderItemValid();
    virtual void setPosition(const Vector3& pos);
    virtual void setRotation(const physx::PxQuat& q);
protected:
    physx::PxTransform _transform;
    std::unique_ptr<RenderItem> _renderItem;
    bool _alive = true;
    physx::PxShape* _shape = nullptr;
    physx::PxRigidActor* _actor = nullptr;
    Vector4 _color;

    bool _renderItemRegistered;
};

