#pragma once
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>
#include <memory>

class SolidEntity
{
public:
    SolidEntity(physx::PxRigidActor* actor, physx::PxShape* shape, Vector4 color)
        : _actor(actor), _shape(shape), _color(color), _renderItem(nullptr), _registered(false)
    {
    }

    ~SolidEntity()
    {
        deregisterRenderItem();
    }

    void createRenderItem()
    {
        deregisterRenderItem();

        _renderItem = std::make_unique<RenderItem>(_shape, _actor, _color);
        RegisterRenderItem(_renderItem.get());
        _registered = true;
    }

    void deregisterRenderItem()
    {
        if (_renderItem && _registered)
        {
            DeregisterRenderItem(_renderItem.get());
            _registered = false;
        }
    }

    bool isRenderItemValid() const
    {
        return _renderItem != nullptr && _registered;
    }
    void setColor(const Vector4& color) {
        _color = color;
        if (_renderItem != nullptr) {
            _renderItem->color = color;
        }
    }

    physx::PxRigidActor* getActor() const { return _actor; }
    physx::PxShape* getShape() const { return _shape; }

private:
    physx::PxRigidActor* _actor;
    physx::PxShape* _shape;

    Vector4 _color;

    std::unique_ptr<RenderItem> _renderItem;
    bool _registered;
};
