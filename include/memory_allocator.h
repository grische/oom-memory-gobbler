#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>
#include <memory>
#include <vector>

class MemoryAllocator {
   private:
    std::vector<std::unique_ptr<std::byte[]>> allocated_chunks;
    size_t total_allocated_mb{0};
    bool verbose_mode{false};

   public:
    explicit MemoryAllocator(bool verbose = false) noexcept;

    [[nodiscard]] bool allocateChunk(size_t size_mb);
    [[nodiscard]] size_t getTotalAllocatedMB() const noexcept;
    void printStats() const;
};

#endif  // MEMORY_ALLOCATOR_H
