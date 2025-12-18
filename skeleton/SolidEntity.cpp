#include "SolidEntity.h"
#include <iostream>

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
void SolidEntity::createPhysicsObject(physx::PxPhysics* physics, physx::PxScene* scene, bool dynamic, const physx::PxVec3& pos, const physx::PxGeometry& geometry, double density, physx::PxMaterial* material)
{
    if (!material)
        material = physics->createMaterial(0.5f, 0.5f, 0.5f);

    physx::PxTransform transform(pos);

    if (dynamic) {
        _actor = physics->createRigidDynamic(transform);
        _shape = physics->createShape(geometry, *material);

        physx::PxFilterData fd;
        fd.word0 = 1; // grupo del actor
        fd.word1 = 2; // m¨¢scara de colisi¨®n
        _shape->setSimulationFilterData(fd);

        _actor->attachShape(*_shape);

        physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>();
        physx::PxRigidBodyExt::updateMassAndInertia(*dyn, density);

    }
    else {
        _actor = physics->createRigidStatic(transform);
        _shape = physics->createShape(geometry, *material);
        _actor->attachShape(*_shape);
    }

    scene->addActor(*_actor);
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())dyn->clearForce();
}

SolidEntity::SolidEntity(
    physx::PxPhysics* physics, physx::PxScene* scene,
    bool dynamic, const physx::PxVec3& pos,
    const physx::PxGeometry& geometry, double density,
    physx::PxMaterial* material, const Vector4& color)
    : Entity(pos, nullptr, color)
{
    _params = new ConstructionParams();
    _params->physics = physics;
    _params->pos = pos;
    _params->dynamic = dynamic;
    _params->density = density;
    _params->material = material;
    _params->geometry = cloneGeometry(geometry);
    _params->scene = scene;
    //createPhysicsObject(physics, scene, dynamic, pos, geometry, density, material);
}
SolidEntity::SolidEntity(const SolidEntity& other, const Vector3& pos)
    : Entity(pos, nullptr, other._color)
{
    _lifetime = other._lifetime;
    _timeAlive = 0.0f;
    _params = new ConstructionParams(*other._params);
    _params->pos = pos;
    if (other._params->geometry)
        _params->geometry = cloneGeometry(*other._params->geometry);

    createPhysicsObject(
        _params->physics,
        _params->scene,
        _params->dynamic,
        pos,
        *_params->geometry,
        _params->density,
        _params->material
    );
}


SolidEntity::~SolidEntity()
{
    deregisterRenderItem();
}

void SolidEntity::update(double dt)
{
    if (_lifetime > 0) {
        _timeAlive += dt;
        if (_timeAlive >= _lifetime) {
            _alive = false;
            std::cout << "SOlido muestro\n";
        }
    }
}

void SolidEntity::create_renderItem()
{
    deregisterRenderItem();
    _renderItem = std::make_unique<RenderItem>(_shape, _actor, _color);
    _renderItemRegistered = true;
}

void SolidEntity::create_physicsObject()
{
    if (_actor) {
        _actor = nullptr;
    };
    createPhysicsObject(_params->physics, _params->scene, _params->dynamic, _params->pos, *_params->geometry, _params->density, _params->material);
}

void SolidEntity::addForce(const Vector3& force)
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>()) {
        dyn->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eFORCE);
    }

}

void SolidEntity::addTorque(const Vector3& torque)
{
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        dyn->addTorque(torque);
}

bool SolidEntity::is_alive() const {
    return _alive && _actor != nullptr;
}

//Getters

double SolidEntity::getMass() const
{

    if (!_actor) {
        return 0.0;
    }
    if (const physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        return dyn->getMass();
    return 0.0f;
}

Vector3 SolidEntity::getVelocity() const
{
    if (const physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        return dyn->getLinearVelocity();
    return Vector3(0, 0, 0);
}

Vector3 SolidEntity::getAngularVelocity() const
{
    if (const physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
        return dyn->getAngularVelocity();
    return Vector3(0, 0, 0);
}

Vector3 SolidEntity::getPos() const
{
    if (_actor) {
        physx::PxTransform t = _actor->getGlobalPose();
        return Vector3(t.p.x, t.p.y, t.p.z);
    }
    return _params->pos;
}

Vector3 SolidEntity::getInertiaTensor() const
{
    if (!_actor) return Vector3(0, 0, 0);

    if (const physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
    {
        return dyn->getMassSpaceInertiaTensor();
    }
}

bool SolidEntity::isDynamic() const {
    return _params ? _params->dynamic : false;
}

physx::PxGeometry* SolidEntity::getGeometry() const {
    return _params ? _params->geometry : nullptr;
}

float SolidEntity::getDensity() const {
    return _params ? _params->density : 1.0f;
}

physx::PxMaterial* SolidEntity::getMaterial() const {
    return _params ? _params->material : nullptr;
}
//Setters

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

void SolidEntity::setLifeTime(double lifetime)
{
    _lifetime = lifetime;
    _timeAlive = 0.0f;
}

void SolidEntity::setInertiaTensor(const Vector3& I)
{
    if (!_actor) return;

    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
    {
        dyn->setMassSpaceInertiaTensor(I);
    }
}

void SolidEntity::setColor(const Vector4& color)
{
    _color = color;
    if (_renderItem && _renderItemRegistered)
        _renderItem->color = color;
}
void SolidEntity::setRotation(const physx::PxQuat& q) {

    if(_actor)
    {
        physx::PxTransform t = _actor->getGlobalPose();
        t.q = q;
        _actor->setGlobalPose(t);
    }
}

physx::PxRigidActor* SolidEntity::getActor() const { return _actor; }
physx::PxShape* SolidEntity::getShape() const { return _shape; }
