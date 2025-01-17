#pragma once

#include <container/seadStrTreeMap.h>
#include <heap/seadExpHeap.h>
#include <heap/seadFrameHeap.h>
#include <heap/seadHeapMgr.h>

namespace al {
class AudioResourceDirector;

class MemorySystem {
public:
    MemorySystem(sead::Heap* heap);

    void allocFailedCallbackFunc(const sead::HeapMgr::AllocFailedCallbackArg*);
    void createSequenceHeap();
    void freeAllSequenceHeap();
    bool printSequenceHeap();
    bool createSceneHeap(const char* stageName, bool backwards);
    void createSceneResourceHeap(const char* stageName, bool backwards);
    void destroySceneHeap();
    void destroySceneResourceHeap();
    void createCourseSelectHeap();
    void destroyCourseSelectHeap();
    void createWorldResourceHeap();
    void destroyWorldResourceHeap();
    void freeAllPlayerHeap();
    sead::Heap* tryFindNamedHeap(const char* heapName) const;
    sead::Heap* findNamedHeap(const char* heapName) const;
    void addNamedHeap(sead::Heap* heap, const char* heapName);
    void removeNamedHeap(const char* heapName);

    sead::ExpHeap* getStationedHeap() { return mStationedHeap; }

    sead::ExpHeap* getSequenceHeap() { return mSequenceHeap; }

    sead::ExpHeap* getSceneResourceHeap() { return mSceneResourceHeap; }

    sead::FrameHeap* getSceneHeap() { return mSceneHeap; }

    sead::ExpHeap* getPlayerResourceHeap() { return mPlayerResourceHeap; }

    sead::ExpHeap* getCourseSelectResourceHeap() { return mCourseSelectResourceHeap; }

    sead::ExpHeap* getCourseSelectHeap() { return mCourseSelectHeap; }

    sead::ExpHeap* getWorldResourceHeap() { return mWorldResourceHeap; }

    AudioResourceDirector* getAudioResourceDirector() { return mAudioResourceDirector; }

    void setAudioResourceDirector(AudioResourceDirector* audioResourceDirector) {
        mAudioResourceDirector = audioResourceDirector;
    }

private:
    sead::ExpHeap* mStationedHeap;
    sead::ExpHeap* mSequenceHeap;
    sead::ExpHeap* mSceneResourceHeap;
    sead::FrameHeap* mSceneHeap;
    sead::ExpHeap* mPlayerResourceHeap;
    sead::ExpHeap* mCourseSelectResourceHeap;
    sead::ExpHeap* mCourseSelectHeap;
    sead::ExpHeap* mWorldResourceHeap;
    sead::StrTreeMap<32, sead::Heap*> mHeapList;
    AudioResourceDirector* mAudioResourceDirector;
    bool mIsExistFileResource;
    sead::Delegate1<MemorySystem, const sead::HeapMgr::AllocFailedCallbackArg*> mDelegate;

    static constexpr size_t cDefaultSceneHeapSize = 490 * 1024 * 1024 /* 490 MB */;
    static constexpr size_t cWorldResourceHeapSize = 855 * 1024 * 1024 /* 855 MB */;
};

static_assert(sizeof(MemorySystem) == 0x90);
}  // namespace al
