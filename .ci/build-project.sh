#!/bin/bash

# Build project.
#
# The script assumes that it will be called from inside the project directory.
#
# Usage: /bin/bash .ci/build-project.sh [build-directory-name]
# - build-directory-name: Optional name of build directory. Default: build.
#
# Example 1: /bin/bash .ci/build-project.sh
# Example 2: /bin/bash .ci/build-project.sh build-clang

set -e

BUILD_DIR=${1:-build}

echo "---- build-project.sh ----"
echo "BUILD_DIR: $BUILD_DIR"
echo "--------------------------"

mkdir "$BUILD_DIR"
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DDST_TESTS:BOOL=ON -B "$BUILD_DIR" -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
cmake --build "$BUILD_DIR" -j
