#include <signal.h>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include "config.h"
#include "memory_allocator.h"

volatile bool running = true;

void signalHandler(int) {
    running = false;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    auto config = parseArgs(argc, argv);
    MemoryAllocator allocator(config.verbose);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> delay_dist(config.min_delay,
                                                  config.max_delay);

    std::cout << "OOM Memory Gobbler starting...\n";

    // Allocation phase
    while (running && allocator.getTotalAllocatedMB() < config.max_memory_mb) {
        // Wait with random delay
        int delay = delay_dist(gen);
        for (int i = 0; i < delay && running; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        if (!running)
            break;

        if (!allocator.allocateChunk(config.chunk_size_mb)) {
            break;
        }
    }

    std::cout << "Final allocation: " << allocator.getTotalAllocatedMB()
              << " MB\n";

    // Hold phase (if requested)
    if (config.hold_memory && running) {
        std::cout << "Holding " << allocator.getTotalAllocatedMB()
                  << "MB of memory. Press Ctrl+C to exit.\n";

        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "\nReceived signal, cleaning up and exiting...\n";
    }

    return 0;
}
