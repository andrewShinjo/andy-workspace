@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul 2>&1

if not exist "build" mkdir "build"

del /Q "build\*.*"

echo "Building build\win32_handmadehero.exe."

pushd build

cl /nologo /Zi /Od /Fe"win32_handmadehero.exe" "..\app\handmadehero\win32_handmadehero.c" User32.lib Gdi32.lib

popd