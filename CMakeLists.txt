cmake_minimum_required(VERSION 3.25)
project(blackjack_real)

set(CMAKE_CXX_STANDARD 17)

add_executable(blackjack_real main.cpp player.h player.cpp table.cpp table.h deck.cpp deck.h tree_and_bar.cpp tree_and_bar.h)
