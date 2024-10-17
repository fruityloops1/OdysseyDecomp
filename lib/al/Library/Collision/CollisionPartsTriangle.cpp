#include "Library/Collision/CollisionPartsTriangle.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"

namespace al {

Triangle::Triangle() : mCollisionParts(nullptr), mKCPrismData(nullptr), mKCPrismHeader(nullptr) {
    // nullptrs above done to avoid initializing them in the other ctor

    mNormals[0] = sead::Vector3f::zero;
    mNormals[1] = sead::Vector3f::zero;
    mNormals[2] = sead::Vector3f::zero;
    mNormals[3] = sead::Vector3f::zero;
    mPositions[0] = sead::Vector3f::zero;
    mPositions[1] = sead::Vector3f::zero;
    mPositions[2] = sead::Vector3f::zero;
}

Triangle::Triangle(const CollisionParts& parts, const KCPrismData* data,
                   const KCPrismHeader* header) {
    fillData(parts, data, header);
}

void Triangle::fillData(const CollisionParts& parts, const KCPrismData* data,
                        const KCPrismHeader* header) {
    mCollisionParts = &parts;
    mKCPrismData = data;
    mKCPrismHeader = header;

    KCollisionServer* kCollisionServer = parts.getKCollisionServer();
    const sead::Matrix34f& baseMtx = parts.getBaseMtx();

    mNormals[0].setRotated(baseMtx, kCollisionServer->getFaceNormal(data, mKCPrismHeader));
    mNormals[1].setRotated(baseMtx, kCollisionServer->getEdgeNormal1(data, mKCPrismHeader));
    mNormals[2].setRotated(baseMtx, kCollisionServer->getEdgeNormal2(data, mKCPrismHeader));
    mNormals[3].setRotated(baseMtx, kCollisionServer->getEdgeNormal3(data, mKCPrismHeader));

    normalize(&mNormals[0]);
    normalize(&mNormals[1]);
    normalize(&mNormals[2]);
    normalize(&mNormals[3]);

    kCollisionServer->calcPosLocal(&mPositions[0], data, 0, mKCPrismHeader);
    kCollisionServer->calcPosLocal(&mPositions[1], data, 1, mKCPrismHeader);
    kCollisionServer->calcPosLocal(&mPositions[2], data, 2, mKCPrismHeader);

    mPositions[0].mul(baseMtx);
    mPositions[1].mul(baseMtx);
    mPositions[2].mul(baseMtx);
}

void Triangle::fill(const sead::Vector3f& pos1, const sead::Vector3f& pos2,
                    const sead::Vector3f& pos3) {
    mPositions[0] = pos1;
    mPositions[1] = pos2;
    mPositions[2] = pos3;

    sead::Vector3f vert1To2 = pos2 - pos1;
    sead::Vector3f vert1To3 = pos3 - pos1;
    sead::Vector3f vert2To3 = pos3 - pos2;

    mNormals[0].setCross(vert1To2, vert1To3);
    tryNormalizeOrZero(&mNormals[0]);

    mNormals[1].setCross(vert1To2, mNormals[0]);
    tryNormalizeOrZero(&mNormals[1]);
    mNormals[2].setCross(mNormals[0], vert1To3);
    tryNormalizeOrZero(&mNormals[2]);
    mNormals[3].setCross(vert2To3, mNormals[0]);
    tryNormalizeOrZero(&mNormals[3]);

    mKCPrismData = nullptr;
}

const LiveActor* Triangle::getHostActor() const {
    if (!mCollisionParts)
        return nullptr;
    return mCollisionParts->getConnectedHost();
}

bool Triangle::isHostMoved() const {
    return mCollisionParts->get_15c() == 0 || mCollisionParts->isMoving();
}

bool Triangle::isValid() const {
    return mKCPrismData != nullptr;
}

const sead::Vector3f& Triangle::getNormal(s32 index) const {
    return mNormals[index];
}

const sead::Vector3f& Triangle::getFaceNormal() const {
    return getNormal(0);
}

const sead::Vector3f& Triangle::getEdgeNormal(s32 index) const {
    return getNormal(index + 1);
}

const sead::Vector3f& Triangle::getPos(s32 index) const {
    return mPositions[index];
}

void Triangle::calcCenterPos(sead::Vector3f* center) const {
    *center = (getPos(0) + getPos(1) + getPos(2)) * (1 / 3.0f);
}

const sead::Vector3f& Triangle::calcAndGetNormal(s32 index) {
    const CollisionParts* collisionParts = mCollisionParts;
    KCollisionServer* kCollisionServer = collisionParts->getKCollisionServer();
    switch (index) {
    case 0:
        return calcAndGetFaceNormal();
    case 1:
        mNormals[1] = kCollisionServer->getEdgeNormal1(mKCPrismData, mKCPrismHeader);
        mNormals[1].setRotated(collisionParts->getBaseMtx(), mNormals[1]);
        normalize(&mNormals[1]);
        return getNormal(index);
    case 2:
        mNormals[2] = kCollisionServer->getEdgeNormal2(mKCPrismData, mKCPrismHeader);
        mNormals[2].setRotated(collisionParts->getBaseMtx(), mNormals[2]);
        normalize(&mNormals[2]);
        return getNormal(index);
    case 3:
        mNormals[3] = kCollisionServer->getEdgeNormal3(mKCPrismData, mKCPrismHeader);
        mNormals[3].setRotated(collisionParts->getBaseMtx(), mNormals[3]);
        normalize(&mNormals[3]);
        return getNormal(index);
    default:
        return getNormal(index);
    }
}

const sead::Vector3f& Triangle::calcAndGetFaceNormal() {
    const CollisionParts* collisionParts = mCollisionParts;
    mNormals[0] =
        collisionParts->getKCollisionServer()->getFaceNormal(mKCPrismData, mKCPrismHeader);
    mNormals[0].setRotated(collisionParts->getBaseMtx(), mNormals[0]);
    normalize(&mNormals[0]);
    return mNormals[0];
}

const sead::Vector3f& Triangle::calcAndGetEdgeNormal(s32 index) {
    const CollisionParts* collisionParts = mCollisionParts;
    KCollisionServer* kCollisionServer = collisionParts->getKCollisionServer();
    switch (index) {
    case 0:
        mNormals[1] = kCollisionServer->getEdgeNormal1(mKCPrismData, mKCPrismHeader);
        mNormals[1].rotate(collisionParts->getBaseMtx());
        normalize(&mNormals[1]);
        return getEdgeNormal(index);
    case 1:
        mNormals[2] = kCollisionServer->getEdgeNormal2(mKCPrismData, mKCPrismHeader);
        mNormals[2].rotate(collisionParts->getBaseMtx());
        normalize(&mNormals[2]);
        return getEdgeNormal(index);
    case 2:
        mNormals[3] = kCollisionServer->getEdgeNormal3(mKCPrismData, mKCPrismHeader);
        mNormals[3].rotate(collisionParts->getBaseMtx());
        normalize(&mNormals[3]);
        return getEdgeNormal(index);
    default:
        return getEdgeNormal(index);
    }
}

const sead::Vector3f& Triangle::calcAndGetPos(s32 index) {
    mCollisionParts->getKCollisionServer()->calcPosLocal(&mPositions[index], mKCPrismData, index,
                                                         mKCPrismHeader);
    mPositions[index].mul(getBaseMtx());
    return getPos(index);
}

void Triangle::getLocalPos(sead::Vector3f* pos, s32 index) const {
    mCollisionParts->getKCollisionServer()->calcPosLocal(pos, mKCPrismData, index, mKCPrismHeader);
}

void Triangle::calcForceMovePower(sead::Vector3f* power, const sead::Vector3f& pos) const {
    mCollisionParts->calcForceMovePower(power, pos);
}

void Triangle::calcForceRotatePower(sead::Quatf* power) const {
    mCollisionParts->calcForceRotatePower(power);
}

bool Triangle::getAttributes(ByamlIter* iter) const {
    if (!isValid())
        return false;
    return mCollisionParts->getKCollisionServer()->getAttributes(iter, mKCPrismData);
}

const HitSensor* Triangle::getSensor() const {
    return mCollisionParts->getConnectedSensor();
}

const sead::Matrix34f& Triangle::getBaseMtx() const {
    return mCollisionParts->getBaseMtx();
}

const sead::Matrix34f& Triangle::getBaseInvMtx() const {
    return mCollisionParts->getBaseInvMtx();
}

const sead::Matrix34f& Triangle::getPrevBaseMtx() const {
    return mCollisionParts->getPrevBaseMtx();
}

}  // namespace al

bool operator==(const al::Triangle& lhs, const al::Triangle& rhs) {
    return lhs.mCollisionParts == rhs.mCollisionParts && lhs.mKCPrismData == rhs.mKCPrismData;
}

bool operator!=(const al::Triangle& lhs, const al::Triangle& rhs) {
    return !(lhs == rhs);
}