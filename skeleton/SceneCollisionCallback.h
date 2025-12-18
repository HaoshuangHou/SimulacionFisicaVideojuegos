#include <PxPhysicsAPI.h>
#include <vector>
#include "SceneGame.h"

struct ForceZone
{
    physx::PxActor* actor;
    ForceType forceType;     
    bool active = false;   
};
class SceneCollisionCallback : public physx::PxSimulationEventCallback {
public:
    SceneCollisionCallback(SceneGame* scene) : _scene(scene), _forceZones(){}

    void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;

    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {}
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {}
    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {}
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override {};
    virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer,
        const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {
    }

    void addForceZones(physx::PxActor* actor, ForceType force, bool active = false);
    void clearForceZones();
private:
    SceneGame* _scene;
    std::vector<ForceZone> _forceZones;
};
