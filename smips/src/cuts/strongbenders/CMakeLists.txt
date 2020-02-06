cmake_minimum_required(VERSION 3.2)

set(SOURCES
		computecut.cpp
		strongbenders.cpp)

add_library(strongbenders ${HEADERS} ${SOURCES})
target_link_libraries(strongbenders
		sub
		lagrangian)
