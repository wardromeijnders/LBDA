cmake_minimum_required(VERSION 3.2)

set(SOURCES
		addcut.cpp
		copyconstructor.cpp
        destructor.cpp
		masterproblem.cpp
		solve.cpp
        )

add_library(master ${SOURCES})
target_link_libraries(master armadillo)
