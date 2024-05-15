cmake_minimum_required(VERSION 3.25)
project(blackjack_real)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Add the executable
add_executable(blackjack_real ${SOURCES})
