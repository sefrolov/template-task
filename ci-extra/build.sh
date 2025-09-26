#!/usr/bin/env bash
set -euo pipefail

PRESET_NAME=$1

# Configure CMake
cmake -S . \
  --preset "${PRESET_NAME}" -G Ninja \
  -D CT_TREAT_WARNINGS_AS_ERRORS=ON

# Build
cmake --build "build/${PRESET_NAME}" -j | tee "build/build_log.out"

# iwyu_tool.py -p "build/${PRESET_NAME}" -j -- -Wno-unknown-warning-option -Xiwyu --error_always || true

if grep -Eq "include-what-you-use reported diagnostics" "build/build_log.out"; then
  exit 1
fi
