cmake_minimum_required(VERSION 3.2)

set(HEADERS
		../../../include/decompositions/cglp.h)

set(SOURCES
		adddisjunction.cpp
		cglp.cpp
		changelastrow.cpp
		destructor.cpp
		solve.cpp)

add_library(cglp ${HEADERS} ${SOURCES})
