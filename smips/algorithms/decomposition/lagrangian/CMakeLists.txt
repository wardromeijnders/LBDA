cmake_minimum_required(VERSION 3.2)

set(HEADERS
		lagrangian.h)

set(SOURCES
		copy_ctor.cpp
		destructor.cpp
		lagrangian.cpp
		solve.cpp
		update.cpp)

add_library(lagrangian ${HEADERS} ${SOURCES})
