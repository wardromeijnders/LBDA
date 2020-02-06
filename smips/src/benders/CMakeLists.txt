cmake_minimum_required(VERSION 3.2)

set(SOURCES
		benders.cpp
		solve.cpp)

add_library(benders ${HEADERS} ${SOURCES})
target_link_libraries(benders
		decompositions
		master
		sub)
