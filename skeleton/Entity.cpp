#include "Entity.h"
#include "RenderUtils.hpp"

Entity::~Entity()
{
	deregisterRenderItem();
}

void Entity::create_renderItem()
{
	deregisterRenderItem();

	_renderItem = std::make_unique<RenderItem>(_shape, &_transform, _color);
	RegisterRenderItem(_renderItem.get());
	_renderItemRegistered = true;
}

void Entity::deregisterRenderItem()
{
	if (_renderItem && _renderItemRegistered) {
		DeregisterRenderItem(_renderItem.get());
		_renderItemRegistered = false;
	}
}

bool Entity::isRenderItemValid()
{
	return _renderItem != nullptr && _renderItemRegistered;
}

void Entity::setPosition(const Vector3& pos)
{
	_transform.p = pos;
}

void Entity::setRotation(const physx::PxQuat& q)
{
	_transform.q = q;
}

Entity::Entity(const Vector3& pos, physx::PxShape* shape, Vector4 color)
	:_alive(true),
	_shape(shape),
	_color(color),
	_transform(physx::PxTransform(pos.x, pos.y, pos.z)),
	_renderItem(nullptr),
	_renderItemRegistered(false)
{
}

Entity::Entity(const Vector3& pos, physx::PxShape* shape, Vector4 color, bool renderItem)
	:_alive(true),
	_shape(shape),
	_color(color),
	_transform(physx::PxTransform(pos.x, pos.y, pos.z)),
	_renderItem(nullptr),
	_renderItemRegistered(false)
{
	if (renderItem)create_renderItem();
}
