#include "SolidEntity.h"

static physx::PxGeometry* cloneGeometry(const physx::PxGeometry& geo) {
    switch (geo.getType()) {
    case physx::PxGeometryType::eBOX:
        return new physx::PxBoxGeometry((const physx::PxBoxGeometry&)geo);
    case physx::PxGeometryType::eSPHERE:
        return new physx::PxSphereGeometry((const physx::PxSphereGeometry&)geo);
    case physx::PxGeometryType::eCAPSULE:
        return new physx::PxCapsuleGeometry((const physx::PxCapsuleGeometry&)geo);
    case physx::PxGeometryType::ePLANE:
        return new physx::PxPlaneGeometry();
    default:
        return nullptr;
    }
}
void SolidEntity::createPhysicsObject(physx::PxPhysics* physics, physx::PxScene* scene, bool dynamic, const physx::PxVec3& pos, const physx::PxGeometry& geometry, float density, physx::PxMaterial* material)
{
    if (!material)
        material = physics->createMaterial(0.5f, 0.5f, 0.5f);

    physx::PxTransform transform(pos);

    if (dynamic) {
        _actor = physics->createRigidDynamic(transform);
        _shape = physics->createShape(geometry, *material);
        _actor->attachShape(*_shape);
        physx::PxRigidBodyExt::updateMassAndInertia(
            *(physx::PxRigidDynamic*)_actor, density);
    }
    else {
        _actor = physics->createRigidStatic(transform);
        _shape = physics->createShape(geometry, *material);
        _actor->attachShape(*_shape);
    }

    scene->addActor(*_actor);

    if (!_params) {
        _params = new ConstructionParams();
    }
    _params->physics = physics;
    _params->scene = scene;
    _params->dynamic = dynamic;
    _params->density = density;
    _params->material = material;
    _params->geometry = cloneGeometry(geometry);

}

SolidEntity::SolidEntity(
    physx::PxPhysics* physics, physx::PxScene* scene,
    bool dynamic, const physx::PxVec3& pos,
    const physx::PxGeometry& geometry, float density,
    physx::PxMaterial* material, const Vector4& color)
    : Entity(pos, nullptr, color)
{
    createPhysicsObject(physics, scene, dynamic, pos, geometry, density, material);
}

SolidEntity::SolidEntity(const SolidEntity& other)
    : Entity(other.getPos(), nullptr, other._color),
    _lifetime(other._lifetime),
    _timeAlive(0.0f)
{
    _color = other._color;
    if (other._params) {
        _params = new ConstructionParams(*other._params);

        if (other._params->geometry)
            _params->geometry = cloneGeometry(*other._params->geometry);

        createPhysicsObject(
            _params->physics,
            _params->scene,
            _params->dynamic,
            other.getPos(),
            *_params->geometry,
            _params->density,
            _params->material
        );

        setVelocity(other.getVelocity());
        setAngularVelocity(other.getAngularVelocity());
    }
}

void SolidEntity::update(double dt)
{
    if (_lifetime > 0) {
        _timeAlive += dt;
        if (_timeAlive >= _lifetime) {
            _alive = false;
        }
    }
}

void SolidEntity::create_renderItem()
{
    deregisterRenderItem();
    _renderItem = std::make_unique<RenderItem>(_shape, _actor, _color);
    _renderItemRegistered = true;
}

void SolidEntity::addForce(const Vector3& force)
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        dyn->addForce(force);
}

void SolidEntity::addTorque(const Vector3& torque)
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        dyn->addTorque(torque);
}

bool SolidEntity::is_alive() const {
    return _alive;
}

// --- Getters ---

double SolidEntity::getMass() const
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        return dyn->getMass();
    return 0.0f;
}

Vector3 SolidEntity::getVelocity() const
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        return dyn->getLinearVelocity();
    return Vector3(0, 0, 0);
}

Vector3 SolidEntity::getAngularVelocity() const
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        return dyn->getAngularVelocity();
    return Vector3(0, 0, 0);
}

Vector3 SolidEntity::getPos() const
{
    physx::PxTransform transform = _actor->getGlobalPose();
    return Vector3(transform.p.x, transform.p.y, transform.p.z);
}

// --- Setters ---

void SolidEntity::setVelocity(const Vector3& vel)
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        dyn->setLinearVelocity(physx::PxVec3(vel.x, vel.y, vel.z));
}

void SolidEntity::setAngularVelocity(const Vector3& angVel)
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        dyn->setAngularVelocity(physx::PxVec3(angVel.x, angVel.y, angVel.z));
}

void SolidEntity::setPos(const Vector3& pos)
{
    if (!_actor) return;

    physx::PxTransform transform = _actor->getGlobalPose();
    transform.p = pos;
    _actor->setGlobalPose(transform);

    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>()) {
        dyn->setLinearVelocity(physx::PxVec3(0, 0, 0));
        dyn->setAngularVelocity(physx::PxVec3(0, 0, 0));
    }
}

void SolidEntity::setLifeTime(double lifetime)
{
    _lifetime = lifetime;
    _timeAlive = 0.0f;
}

void SolidEntity::setColor(const Vector4& color)
{
    _color = color;
    if (_renderItem && _renderItemRegistered)
        _renderItem->color = color;
}

physx::PxRigidActor* SolidEntity::getActor() const { return _actor; }
physx::PxShape* SolidEntity::getShape() const { return _shape; }
