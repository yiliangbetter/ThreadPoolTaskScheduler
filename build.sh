#!/bin/bash

# Build script for ThreadPoolTaskScheduler

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Run CMake and build
cmake ..
cmake --build .

echo "Build completed successfully."