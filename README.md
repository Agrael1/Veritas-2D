# Veritas Engine
[![CMake](https://github.com/Agrael1/Veritas-2D/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Agrael1/Veritas-2D/actions/workflows/cmake.yml)

Once it was a test of C language and a platform to learn C++, but now it became a challenge project of itself, which has proper graphics system, useful C libraries and a Physics Engine. How far can we go with C?

# Current Features:
## VLib:
 - Classes and OOP simulation
 - Templates in C! Currently used in containers
 - STL C++ containers and optimized strings
## Media Player:
 - Windows Media Player, that exports frame textures using d3d11, has API for C++ and C
## VEngine
 - Graphical engine in Win cmd, has distinct C class api, to consume OnUserCreate, OnUserUpdate and OnUserDestroy should be set as a function pointers

# Requirements:
 - CMake 3.15 and higher
 - Windows 10

# Installation:
For now only a Windows build is possible, in future there may be pixel game engine made in Vulkan or WebGPU

Build with CMake, executable projects are listed below
Libraries have their headers, documentation and binaries stored in respective build directories and are ready to be consumed

# Applications:
 - Bad Apple: plays black and white video using windows command prompt

# Features to go:
- [ ] Build system
- [x] WinRT player to cmake
- [ ] Restore old game in CGE
- [ ] Tests for containers
- [x] Workflow
- [ ] Interfacing to pixel graphics
- [ ] Remove .sln
- [ ] Documentation
