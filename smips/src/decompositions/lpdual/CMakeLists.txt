cmake_minimum_required(VERSION 3.2)

set(SOURCES
		computecut.cpp
		lpdual.cpp)

add_library(lpdual ${HEADERS} ${SOURCES})
target_link_libraries(lpdual sub)
