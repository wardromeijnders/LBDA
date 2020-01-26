cmake_minimum_required(VERSION 3.2)

set(SOURCES
		benders.cpp
		computegomory.cpp
		computetx.cpp
		copyconstructor.cpp
		destructor.cpp
		lbda.cpp
		lbdacut.cpp
		lpcut.cpp
		lpsolve.cpp
		sbcut.cpp
		strongbenders.cpp)

add_library(benders ${HEADERS} ${SOURCES})
target_link_libraries(benders
		decompositions
		master
		sub)
