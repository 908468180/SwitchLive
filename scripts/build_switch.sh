#!/bin/bash
set -e

BUILD_DIR=cmake-build-switch

cd "$(dirname $0)/.."
git config --global --add safe.directory "$(pwd)"

cmake -B ${BUILD_DIR} -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DPLATFORM_SWITCH=ON

cmake --build ${BUILD_DIR} -j$(nproc)
