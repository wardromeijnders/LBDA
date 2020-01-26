cmake_minimum_required(VERSION 3.2)

set(HEADERS
		../../../include/decompositions/gomory.h)

set(SOURCES
		copyconstructor.cpp
		destructor.cpp
		gomory.cpp
		settimelimit.cpp
		solve.cpp
		update.cpp)

add_library(gomory ${HEADERS} ${SOURCES})
