cmake_minimum_required(VERSION 3.2)

set(HEADERS
		../../../include/decompositions/cglp.h)

set(SOURCES
		add_disjunction.cpp
		cglp.cpp
		chg_row.cpp
		destructor.cpp
		solve.cpp)

add_library(cglp ${HEADERS} ${SOURCES})
