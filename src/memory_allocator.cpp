#include "memory_allocator.h"
#include <sys/resource.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

MemoryAllocator::MemoryAllocator(bool verbose) noexcept
    : verbose_mode(verbose) {}

bool MemoryAllocator::allocateChunk(size_t size_mb) {
    size_t bytes = size_mb * 1024 * 1024;

    try {
        auto chunk = std::make_unique<std::byte[]>(bytes);
        std::memset(chunk.get(), 0xAA, bytes);  // Touch memory

        allocated_chunks.push_back(std::move(chunk));
        total_allocated_mb += size_mb;

        std::cout << "Allocated " << size_mb
                  << " MB (Total: " << total_allocated_mb << " MB)\n";
        if (verbose_mode) {
            printStats();
        }

        return true;
    } catch (const std::bad_alloc&) {
        std::cerr << "Failed to allocate " << size_mb << " MB\n";
        return false;
    }
}

size_t MemoryAllocator::getTotalAllocatedMB() const noexcept {
    return total_allocated_mb;
}

void MemoryAllocator::printStats() const {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        std::cout << "  RSS: " << usage.ru_maxrss / 1024 << " MB\n";
    }
}
