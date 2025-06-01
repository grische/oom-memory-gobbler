#ifndef CONFIG_H
#define CONFIG_H

#include <cstddef>

struct Config {
    size_t chunk_size_mb = 0;
    size_t max_memory_mb = 1000;
    int min_delay = 1;
    int max_delay = 5;
    bool verbose = false;
    bool hold_memory = false;
};

Config parseArgs(int argc, char* argv[]);

#endif  // CONFIG_H
