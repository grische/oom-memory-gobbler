#include "config.h"
#include <getopt.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

// Helper function to parse size with suffixes
size_t parseSizeWithSuffix(const char* str) {
    char* endptr;
    long value = std::strtol(str, &endptr, 10);

    if (value < 0) {
        return 0;
    }

    // Check for suffix
    if (*endptr != '\0') {
        char suffix = std::toupper(*endptr);
        switch (suffix) {
            case 'T':
                return static_cast<size_t>(value) * 1024 * 1024;
            case 'G':
                return static_cast<size_t>(value) * 1024;
            case 'M':
                return static_cast<size_t>(value);
            default:
                // Invalid suffix, return 0 to indicate error
                return 0;
        }
    }

    // No suffix, assume MB
    return static_cast<size_t>(value);
}

// Helper function to display usage information
void showHelp(const char* program_name) {
    std::cout << "Usage: " << program_name << " [-s size[G|M|T]]"
              << " [-m max[G|M|T]]" << " [-d min-max|-d delay]" << " [-v]"
              << " [-k]";
    std::cout << std::endl;
    std::cout << "  -m: Max memory (e.g., 1G, 500M)";
    std::cout << std::endl;
    std::cout << "  -s: Chunk size (e.g., 30M, 2G) [default: 10% of max mem]";
    std::cout << std::endl;
    std::cout << "  -d: Delay in seconds (e.g. 1) or delay range (e.g., 1-5)";
    std::cout << std::endl;
    std::cout << "  -v: Verbose output";
    std::cout << std::endl;
    std::cout << "  -k: Keep memory allocated and wait (don't exit)";
    std::cout << std::endl;
}

Config parseArgs(int argc, char* argv[]) {
    Config config{};

    if (argc == 1) {
        showHelp(argv[0]);
        exit(1);
    }

    int opt;
    while ((opt = getopt(argc, argv, "s:m:d:vkh")) != -1) {
        switch (opt) {
            case 'm': {
                size_t size = parseSizeWithSuffix(optarg);
                if (size == 0) {
                    std::cerr << "Invalid memory size format: " << optarg
                              << std::endl;
                    exit(1);
                }
                config.max_memory_mb = size;
                break;
            }
            case 's': {
                size_t size = parseSizeWithSuffix(optarg);
                if (size == 0) {
                    std::cerr << "Invalid size format: " << optarg << std::endl;
                    exit(1);
                }
                config.chunk_size_mb = size;
                break;
            }
            case 'd': {
                if (auto* dash = strchr(optarg, '-')) {
                    config.min_delay = std::atoi(optarg);
                    config.max_delay = std::atoi(dash + 1);
                } else {
                    config.min_delay = std::atoi(optarg);
                    config.max_delay = config.min_delay;
                }
                break;
            }
            case 'v':
                config.verbose = true;
                break;
            case 'k':
                config.hold_memory = true;
                break;
            case 'h':
                showHelp(argv[0]);
                exit(0);
            default:
                showHelp(argv[0]);
                exit(1);
        }
    }

    // Set default chunk size to 10% of max memory if not explicitly set
    if (config.chunk_size_mb == 0) {
        config.chunk_size_mb = config.max_memory_mb / 10;
        if (config.chunk_size_mb == 0) {
            config.chunk_size_mb = 1;
        }
    }

    return config;
}
