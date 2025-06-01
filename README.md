# OOM Memory Gobbler

A C++ Linux command-line tool that gradually increases memory pressure to test the OOM infrastructure.

## Features

- Gradual memory allocation with random delays
- Configurable chunk sizes and maximum memory usage
- Random delay intervals between allocations
- Graceful shutdown on SIGINT/SIGTERM
- Verbose mode with memory statistics
- Command-line argument parsing
- Option to maintain memory allocation until explicitly terminated

## Requirements

- **g++** with C++17 support
- **make**

## Quick Start

### Building and Installation

```bash
git clone https://github.com/grische/oom-memory-gobbler
cd oom-memory-gobbler

make
# The executable will be created as ./oom-memory-gobbler
```

## Usage

```bash
./oom-memory-gobbler [OPTIONS]
```

### Options

- `-s SIZE[G|M|T]`: Size of each memory chunk (e.g., 30M, 2G) [default: 10% of max memory]
- `-m SIZE[G|M|T]`: Maximum memory to allocate (e.g., 1G, 500M) [default: 1000M]
- `-d DELAY`: Single delay value in seconds or delay range (e.g., 1 or 1-5) [default: 1-5]
- `-v`: Enable verbose output with memory statistics
- `-k`: Keep memory allocated and wait (don't exit until Ctrl+C)
- `-h`: Show help message

### Examples

```bash
# Default behavior: 10MB chunks, up to 1000MB, 1-5 second delays
./oom-memory-gobbler

# Custom configuration: 50MB chunks, up to 2000MB, 2-8 second delays
./oom-memory-gobbler -s 50M -m 2000M -d 2-8 -v

# Small increments with fixed 1s delay
./oom-memory-gobbler -s 5M -m 500M -d 1 -v

# Hold memory after allocation (don't exit until Ctrl+C)
./oom-memory-gobbler -s 50M -m 500M -k
```

## How It Works

1. Allocates memory in configurable chunks using `malloc()`
2. Touches allocated memory with `memset()` to ensure actual allocation
3. Waits random intervals between allocations (1-5 seconds by default)
4. Continues until reaching maximum memory limit or receiving termination signal
5. With `-k` option, holds the allocated memory until receiving SIGTERM/SIGINT (Ctrl+C)
6. Gracefully cleans up all allocated memory on exit

## Development and Contributing

### Development Requirements

- **clang-format** for code formatting
- **pre-commit** for automated formatting

### Development Setup

Install and configure pre-commit hooks for code formatting:

```bash
pipx install pre-commit
pre-commit install
```

### Build Commands

```bash
# Debug build with debugging symbols
make debug

# Format source code
make format

# Clean build artifacts
make clean
```
