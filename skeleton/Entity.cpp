#include "Entity.h"
#include "RenderUtils.hpp"

Entity::~Entity()
{
	DeregisterRenderItem(_renderItem);
	delete _renderItem;
}

Entity::Entity(Vector3 pos, physx::PxShape* _shape, Vector4 color)
	:_alive(true)
{
	_transform = physx::PxTransform(pos.x, pos.y, pos.z);
	_renderItem = new RenderItem(_shape, &_transform, color);
	RegisterRenderItem(_renderItem);
}
