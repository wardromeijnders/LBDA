cmake_minimum_required(VERSION 3.2)

set(SOURCES
		decomposition.cpp
		loosebenders.cpp
		lpdual.cpp
		strongbenders.cpp)

add_library(decompositions ${SOURCES})
