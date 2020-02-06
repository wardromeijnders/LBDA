cmake_minimum_required(VERSION 3.2)

set(HEADERS
		../../../include/decompositions/relaxation.h)

set(SOURCES
		relaxation.cpp
		solve.cpp)

add_library(relaxation ${HEADERS} ${SOURCES})
