#pragma once

#include <common/aglGPUCommon.hpp>
#include <heap/seadHeap.h>
#include "common/aglGPUMemAddr.h"
#include "container/seadPtrArray.h"
#include "prim/seadSafeString.h"

namespace al {

class GpuMemAllocator {
public:
    struct Block {
        agl::GPUMemAddr<void> mAddr;
        void* _18;
        void* _20;
        void* _28;
        void* _30;
        sead::FixedSafeString<32> mName;
        int _70;
    };

    GpuMemAllocator();
    ~GpuMemAllocator();

    void createMemory(const char* name, int, sead::Heap* heap, int, agl::MemoryAttribute attribute);
    agl::GPUMemAddrBase* findGpuMemInfo(const char* name);
    void createMemoryWithTmp(const char* name, int, int, sead::Heap* heap, int,
                             agl::MemoryAttribute attribute);
    void allocMemory(const char* name, int, int);
    void getTempMemoryAddr(const char* name);
    void getTempMemorySize(const char* name);

private:
    sead::PtrArray<Block> mBlocks;

    constexpr static int cBlockCapacity = 0x10;
};

}  // namespace al
