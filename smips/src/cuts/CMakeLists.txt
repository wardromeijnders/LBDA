cmake_minimum_required(VERSION 3.2)

add_subdirectory(cut)
add_subdirectory(loosebenders)
add_subdirectory(strongbenders)
add_subdirectory(lpdual)

add_library(cuts empty.cpp)
target_link_libraries(cuts
		loosebenders
		strongbenders
		lpdual
		cut
		decompositions
		sub)
