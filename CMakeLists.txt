# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
cmake_minimum_required (VERSION 3.10)

project(Sparrow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_TOOLCHAIN_FILE "D:/Program Files/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Include sub-projects.
add_subdirectory("Sparrow")
