#include "Shooter.h"
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>

using namespace physx;

Shooter::Shooter(const Vector3& pos, const Vector4& color)
	: Entity(pos, CreateShape(PxBoxGeometry(2.0f, 0.5f, 0.5f)), { 0.12f, 0.25f, 0.59f, 1.0f }),
	_basePosition(pos), _currentAngle(0)
{

};

void Shooter::update(double t)
{
}

void Shooter::setAngle(float angle)
{
	_currentAngle = angle;

	physx::PxQuat rotation(_currentAngle, physx::PxVec3(0, 0, 1));
	_transform.q = rotation;
}

void Shooter::rotate(float deltaAngle)
{
	setAngle(_currentAngle + deltaAngle);
}

Vector3 Shooter::getPosition() const
{
	const Vector3 direction = getShootDirection();
	const Vector3 cannonPos = Vector3(_transform.p.x, _transform.p.y, _transform.p.z);
	return cannonPos + direction * 2.0f;
}

Vector3 Shooter::getShootDirection() const
{
	return Vector3(cos(_currentAngle), sin(_currentAngle), 0);
}
