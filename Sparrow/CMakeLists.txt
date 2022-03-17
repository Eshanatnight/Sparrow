# CMakeList.txt : CMake project for Corth, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable ("${PROJECT_NAME}"
        "src/Sparrow.cpp"
        "src/Sparrow.h"
        "src/main.cpp"
        "src/Logging.cpp"
        "src/Logging.h")

