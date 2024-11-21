#include "MemorySystem.h"
#include "Library/Memory/HeapUtil.h"

namespace al {

void MemorySystem::addNamedHeap(sead::Heap* heap, const char* heapName) {
    mHeapList.insert(heapName ?: heap->getName().cstr(), heap);
}

bool MemorySystem::createSceneHeap(const char* stageName, bool backwards) {
    if (mSceneResourceHeap == nullptr)
        createSceneResourceHeap(stageName, backwards);

    u64 heapSize = 0x1ea00000;
    sead::Heap::HeapDirection heapDirection =
        backwards ? sead::Heap::cHeapDirection_Reverse : sead::Heap::cHeapDirection_Forward;

    sead::Heap* heap = getCurrentHeap();
    if (heap != nullptr && heap->getMaxAllocatableSize(8) < heapSize)
        heapSize = heap->getMaxAllocatableSize(8);

    mSceneHeap = sead::FrameHeap::create(heapSize, "SceneHeap", nullptr, 8, heapDirection, false);
    mSceneHeap->enableWarning(true);
    return mSceneResourceHeap == nullptr;
}

}  // namespace al
