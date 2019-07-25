# CMakeList.txt : CMake project for a5, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(Boost)

file(GLOB SRC
    "*.h"
    "*.cpp"
)

# Add source to this project's executable.
add_executable (a5 ${SRC})

# TODO: Add tests and install targets if needed.
