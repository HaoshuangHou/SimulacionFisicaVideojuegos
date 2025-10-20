#include "Entity.h"
#include "RenderUtils.hpp"

Entity::~Entity()
{
	if (_renderItem) {
		DeregisterRenderItem(_renderItem.get());
	}
}

void Entity::create_renderItem()
{
	if (_renderItem) {
		DeregisterRenderItem(_renderItem.get());
	}

	_renderItem = std::make_unique<RenderItem>(_shape, &_transform, _color);
	RegisterRenderItem(_renderItem.get());
}

Entity::Entity(Vector3 pos, physx::PxShape* shape, Vector4 color)
	:_alive(true),
	_shape(shape),
	_color(color),
	_transform(physx::PxTransform(pos.x, pos.y, pos.z))
{
}
