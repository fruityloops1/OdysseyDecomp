#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class CollisionParts;
class LiveActor;
class MtxConnector;

MtxConnector* createMtxConnector(const LiveActor* actor);
MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& info);
void attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor, bool);
void attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor,
                                   const sead::Vector3f&, const sead::Vector3f&);
CollisionParts* attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor,
                                              f32, f32);
void connectPoseQT(LiveActor* actor, const MtxConnector* mtxConnector, const sead::Quatf& quat,
                   const sead::Vector3f& trans);
void connectPoseQT(LiveActor* actor, const MtxConnector* mtxConnector);
void attachMtxConnectorToJoint(MtxConnector* mtxConnector, const LiveActor* actor,
                               const char* jointName);
void attachMtxConnectorToJoint(MtxConnector* mtxConnector, const LiveActor* actor,
                               const char* jointName, const sead::Vector3f& quatInitVec,
                               const sead::Vector3f& trans);
void disconnectMtxConnector(MtxConnector* mtxConnector);
bool isMtxConnectorConnecting(const MtxConnector* mtxConnector);

class CollisionPartsConnector /*: public MtxConnector*/ {
public:
    CollisionPartsConnector();

    void init(sead::Matrix34f* outMtx, const sead::Matrix34f& inMtx,
              const CollisionParts* collisionParts);
    void* getConnectingSensor() const;
    bool isConnecting() const;
    bool isMoved() const;
    bool isConnectInvalidCollision() const;
    void clear();
};

}  // namespace al
