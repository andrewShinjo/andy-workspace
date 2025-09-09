#!/bin/bash

# Exit immediately if any command fails.
set -e

rm -rf build/*
mkdir -p build

echo "Building SDL2 library."

if [ ! -d thirdparty/SDL2/build ]; then
  mkdir -p thirdparty/SDL2/build
  (
    cd thirdparty/SDL2/build
    # Build SDL2 as a shared library
    cmake -DSDL_SHARED=ON -DSDL_STATIC=OFF ..
    make
  )
fi

mkdir -p build/demo

echo "Building demo/sdl2_plaintext-editor"

clang demo/plaintext-editor/sdl2_main.c \
  -Ithirdparty/SDL2/include \
  -Lthirdparty/SDL2/build \
  -lSDL2 \
  -o build/demo/sdl2_plaintext-editor

mkdir -p build/app

echo "Building app/handmadehero."

clang app/handmadehero/sdl2_main.c \
  -Ithirdparty/SDL2/include \
  -Lthirdparty/SDL2/build \
  -lSDL2 \
  -o build/app/sdl2_handmadehero
