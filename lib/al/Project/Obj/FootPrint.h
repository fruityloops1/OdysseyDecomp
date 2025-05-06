#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {

class CollisionParts;

class FootPrint : public LiveActor {
public:
    FootPrint(const ActorInitInfo& info, const char* name);

    void startDisappear();
    bool isDisappear() const;

    void setAnimationByCharacter(const char* name);
    void setAnimationByMaterial(const char* name);
    void setAnimationByMetamorphosis(const char* name);

    void setFollowCollisionParts(CollisionParts* toFollow);

    void appear() override;
    void control() override;

    void exeAppear();
    void exeDisappear();
};

}  // namespace al
