#include "GpuMemAllocator.h"

namespace al {

GpuMemAllocator::GpuMemAllocator() {
    mBlocks.allocBuffer(cBlockCapacity, nullptr);
}

GpuMemAllocator::~GpuMemAllocator() {
    for (int i = 0; i < mBlocks.size(); i++)
        Block* block = mBlocks[i];
}

}  // namespace al
