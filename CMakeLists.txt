cmake_minimum_required(VERSION 3.25)
project(blackjack_real)

set(CMAKE_CXX_STANDARD 17)

# Add all source files in the src folder
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add all header files in the src folder
file(GLOB_RECURSE HEADERS "src/*.h")

# Add the main.cpp file separately
set(MAIN_SRC "src/main.cpp")

# Exclude main.cpp from the list of sources
list(REMOVE_ITEM SOURCES "${MAIN_SRC}")

# Add the executable
add_executable(blackjack_real ${SOURCES} ${HEADERS} ${MAIN_SRC})
