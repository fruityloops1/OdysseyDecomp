#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

class MtxConnector {
    sead::Matrix34f mMtx = sead::Matrix34f::ident;
    void* _38 = nullptr;
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = sead::Vector3f::zero;

public:
    MtxConnector();
    MtxConnector(const sead::Quatf& quat, const sead::Vector3f& trans);

    void init(sead::Matrix34f* outMtx, const sead::Matrix34f& inMtx);
    void multVec(sead::Vector3f* outVec, const sead::Vector3f& vec) const;
    void multTrans(sead::Vector3f* outVec, const sead::Vector3f& trans) const;
    void multMtx(sead::Matrix34f* outMtx, const sead::Matrix34f& mtx) const;
    void multQT(sead::Quatf* outQuat, sead::Vector3f* outTrans, sead::Vector3f* outVec) const;
    void multQT(sead::Quatf* outQuat, sead::Vector3f* outTrans, sead::Vector3f* outVec,
                const sead::Quatf& quat, const sead::Vector3f& vec) const;
    void multQT(sead::Quatf* outQuat, sead::Vector3f* outTrans, const sead::Quatf& quat,
                const sead::Vector3f& vec) const;
    const sead::Quatf& getBaseQuat() const;
    const sead::Vector3f& getBaseTrans() const;
    void setBaseQuatTrans(const sead::Quatf& quat, const sead::Vector3f& trans);
    void calcConnectInfo(sead::Vector3f* outTrans, sead::Quatf* outQuat, sead::Vector3f* outVec,
                         const sead::Vector3f& trans, const sead::Vector3f& vec) const;
    void calcMtxWithOffset(sead::Matrix34f* outMtx, const sead::Vector3f& trans,
                           const sead::Vector3f& vec);
    bool tryGetTargetTrans(sead::Vector3f* outTrans) const;

    virtual bool isConnecting() const;
    virtual void clear();
};

}  // namespace al
