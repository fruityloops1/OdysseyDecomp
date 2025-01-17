#include "Project/Memory/MemorySystem.h"

#include "Library/Memory/HeapUtil.h"
#include "heap/seadExpHeap.h"

namespace al {

bool MemorySystem::createSceneHeap(const char* stageName, bool backwards) {
    sead::ExpHeap* sceneResourceHeap = mSceneResourceHeap;
    if (sceneResourceHeap == nullptr)
        createSceneResourceHeap(stageName, backwards);

    u64 heapSize = cDefaultSceneHeapSize;
    sead::Heap::HeapDirection heapDirection =
        backwards ? sead::Heap::cHeapDirection_Reverse : sead::Heap::cHeapDirection_Forward;

    sead::Heap* heap = getCurrentHeap();
    if (heap != nullptr && heap->getMaxAllocatableSize(8) < heapSize)
        heapSize = heap->getMaxAllocatableSize(8);

    mSceneHeap = sead::FrameHeap::create(heapSize, "SceneHeap", nullptr, 8, heapDirection, false);
    mSceneHeap->enableWarning(false);
    return sceneResourceHeap == nullptr;
}

void MemorySystem::createWorldResourceHeap() {
    mWorldResourceHeap =
        sead::ExpHeap::create(cWorldResourceHeapSize, "WorldHeapResource", mSequenceHeap, 8,
                              sead::ExpHeap::cHeapDirection_Forward, true);
}

void MemorySystem::destroyWorldResourceHeap() {
    mWorldResourceHeap->destroy();
}

void MemorySystem::freeAllPlayerHeap() {
    mPlayerResourceHeap->freeAll();
}

sead::Heap* MemorySystem::tryFindNamedHeap(const char* heapName) const {
    return findNamedHeap(heapName);
}

sead::Heap* MemorySystem::findNamedHeap(const char* heapName) const {
    auto* node = mHeapList.find(heapName);
    return node != nullptr ? node->value() : nullptr;
}

void MemorySystem::addNamedHeap(sead::Heap* heap, const char* heapName) {
    mHeapList.insert(heapName ?: heap->getName().cstr(), heap);
}

}  // namespace al
