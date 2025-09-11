#!/bin/bash

# Exit immediately if any command fails.
set -e

rm -rf build
# rm -rf thirdparty/SDL2/build
# rm -rf thirdparty/SDL2_ttf/build
mkdir -p build

echo "Building SDL2 library."

if [ ! -d thirdparty/SDL2/build ]; then
  mkdir -p thirdparty/SDL2/build
  (
    cd thirdparty/SDL2/build
    cmake -DSDL_SHARED=ON -DSDL_STATIC=OFF ..
    make
  )
fi

echo "Building SDL2_ttf library."

if [ ! -d thirdparty/SDL2_ttf/build ]; then
  mkdir -p thirdparty/SDL2_ttf/build
  (
    cd thirdparty/SDL2_ttf/build
    cmake .. -DCMAKE_C_COMPILER=clang \
         -DCMAKE_BUILD_TYPE=Release \
         -DSDLTTF_VENDORED=ON \
         -DBUILD_SHARED_LIBS=OFF
    cmake --build . --config Release
  )
fi

mkdir -p build/demo

echo "Building demo/sdl2_plaintext_editor"

clang demo/plaintext_editor/sdl2_main.c \
  `sdl2-config --cflags --libs` \
  -lSDL2_ttf \
  -o build/demo/sdl2_plaintext_editor

mkdir -p build/app

echo "Building app/handmadehero."

clang app/handmadehero/sdl2_main.c \
  -Ithirdparty/SDL2/include \
  -Lthirdparty/SDL2/build \
  -lSDL2 \
  -o build/app/sdl2_handmadehero
