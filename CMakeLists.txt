# CMakeList.txt : CMake project for a5, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS thread filesystem)

file(GLOB SRC
    "*.h"
    "*.cpp"
)

# Add source to this project's executable.
add_executable (a5 ${SRC})

target_link_libraries(a5 Boost::filesystem Boost::thread)

# TODO: Add tests and install targets if needed.
