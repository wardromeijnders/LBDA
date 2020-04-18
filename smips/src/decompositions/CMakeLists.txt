cmake_minimum_required(VERSION 3.2)

add_subdirectory(decomposition)
add_subdirectory(loosebenders)
add_subdirectory(strongbenders)
add_subdirectory(lpdual)

add_library(decompositions empty.cpp)
target_link_libraries(decompositions
		loosebenders
		strongbenders
		lpdual
        decomposition
		sub)
