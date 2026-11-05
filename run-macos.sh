#!/bin/bash
set -eu

cd "$(dirname "$0")"

qt_prefix="${QT_PREFIX:-}"
if [ -z "$qt_prefix" ] && command -v brew >/dev/null 2>&1; then
    qt_prefix="$(brew --prefix qt 2>/dev/null || true)"
fi
if [ -z "$qt_prefix" ]; then
    for qt_candidate in "$HOME"/Qt/*/macos; do
        if [ -f "$qt_candidate/lib/cmake/Qt6/Qt6Config.cmake" ] ||
           [ -f "$qt_candidate/lib/cmake/Qt5/Qt5Config.cmake" ]; then
            qt_prefix="$qt_candidate"
        fi
    done
fi

cmake_bin="$(command -v cmake || true)"
if [ -z "$cmake_bin" ]; then
    qt_cmake="$HOME/Qt/Tools/CMake/CMake.app/Contents/bin/cmake"
    if [ -x "$qt_cmake" ]; then
        cmake_bin="$qt_cmake"
    fi
fi
if [ -z "$cmake_bin" ]; then
    echo "CMake не найден. Откройте CMakeLists.txt в Qt Creator или установите CMake."
    exit 1
fi

if [ -n "$qt_prefix" ]; then
    "$cmake_bin" -S . -B build-macos -DCMAKE_PREFIX_PATH="$qt_prefix"
else
    "$cmake_bin" -S . -B build-macos
fi
"$cmake_bin" --build build-macos --parallel 2
open build-macos/WatermelonScene.app
