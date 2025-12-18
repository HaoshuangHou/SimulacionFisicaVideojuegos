#include "SceneCollisionCallback.h"

void SceneCollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader,
    const physx::PxContactPair* pairs,
    physx::PxU32 nbPairs)
{
    physx::PxActor* a0 = pairHeader.actors[0];
    physx::PxActor* a1 = pairHeader.actors[1];

    for (auto& zone : _forceZones) {
        if (zone.actor == a0 || zone.actor == a1) {
            zone.active = !zone.active;
            _scene->setForceActive(zone.forceType, zone.active);
            break;
        }
    }

    SolidProjectil* proj = _scene->getProjectileFromActor(a0);
    SolidEntity* target = _scene->getTargetFromActor(a1);

    if (!proj) {
        proj = _scene->getProjectileFromActor(a1);
        target = _scene->getTargetFromActor(a0);
    }

    if (proj && target) {
        _scene->onProjectileHitTarget(proj, target);
    }
    
}

void SceneCollisionCallback::addForceZones(physx::PxActor* actor, ForceType force, bool active)
{
    _forceZones.push_back({ actor, force, active });
}