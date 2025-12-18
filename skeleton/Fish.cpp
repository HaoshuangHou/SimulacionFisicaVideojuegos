#include "Fish.h"
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>

Fish::Fish(const Vector3& pos, const Vector4& color, float scale)
    : Entity(pos, nullptr,color), _parts(NUM_PARTS)
{
    bodyOffset*= scale;
    tailOffset *= scale;
    eyeOffset *= scale;
    mouthOffset *= scale;

    _transform.p = pos;

    Vector4 colorBody = color;

    Vector4 colorTail = Vector4(color.x * 0.8f, color.y * 0.8f, color.z * 0.8f, color.w);

    Vector4 colorMouth = Vector4(min(color.x * 1.2f, 1.0f),
                                 min(color.y * 1.2f, 1.0f),
                                 min(color.z * 1.2f, 1.0f),
                                 color.w);
    Vector4 colorEye(0.0f, 0.0f, 0.0f, 1.0f);

    const physx::PxQuat bodyRotation(physx::PxPi / 4.0f, physx::PxVec3(0, 0, 1));

    // Cuerpo
    _parts.at(BODY) = std::make_unique<Entity>(
        pos + bodyOffset,
        CreateShape(physx::PxBoxGeometry(1.0f * scale, 1.0f * scale, 0.001f)),
        colorBody);
    _parts.at(BODY)->setRotation(bodyRotation);

    // Cola
    _parts.at(TAIL) = std::make_unique<Entity>(
        pos + tailOffset,
        CreateShape(physx::PxBoxGeometry(0.3f * scale, 0.3f * scale, 0.001f)),
        colorTail);
    _parts.at(TAIL)->setRotation(bodyRotation);

    // Ojos
    _parts.at(EYE) = std::make_unique<Entity>(
        pos + eyeOffset,
        CreateShape(physx::PxSphereGeometry(0.15f * scale)),
        colorEye);

    // Boca
    _parts.at(MOUTH) = std::make_unique<Entity>(
        pos + mouthOffset,
        CreateShape(physx::PxSphereGeometry(0.3f * scale)),
        colorMouth);
}

Fish::~Fish()
{
    _parts.clear();
}

void Fish::setAngle(float angle)
{
    _currentAngle = angle;

    _transform.q = physx::PxQuat(_currentAngle, physx::PxVec3(0, 0, 1));

    updateParts();
}

void Fish::rotate(float deltaAngle)
{
    setAngle(_currentAngle + deltaAngle);
}

void Fish::create_renderItem()
{
    for (auto& p : _parts) {
        p->create_renderItem();
    }
}

void Fish::deregisterRenderItem()
{
    for (auto& p : _parts) {
        p->deregisterRenderItem();
    }
}
void Fish::updateParts()
{
    float cosA = cos(_currentAngle);
    float sinA = sin(_currentAngle);

    Vector3 basePos = _transform.p;

    // Rotar posiciones
    auto rotatePos = [&](const Vector3& offset) {
        return Vector3(
            basePos.x + offset.x * cosA - offset.y * sinA,
            basePos.y + offset.x * sinA + offset.y * cosA,
            basePos.z + offset.z
        );
        };

    // Aplicar posiciones
    _parts[TAIL]->setPosition(basePos);
    _parts[BODY]->setPosition(rotatePos(bodyOffset));
    _parts[EYE]->setPosition(rotatePos(eyeOffset));
    _parts[MOUTH]->setPosition(rotatePos(mouthOffset));

    // Aplicar rotaciones
    physx::PxQuat partRotation(_currentAngle + physx::PxPi / 4.0f, physx::PxVec3(0, 0, 1));
    _parts[BODY]->setRotation(partRotation);
    _parts[TAIL]->setRotation(partRotation);
}
Vector3 Fish::getPosition() const
{
    return _parts[MOUTH]->getPos();
}
void Fish::setPosition(const Vector3& pos)
{
    _transform.p = pos;
    updateParts();
}
Vector3 Fish::getShootDirection() const
{
    return Vector3(cos(_currentAngle), sin(_currentAngle), 0);
}
