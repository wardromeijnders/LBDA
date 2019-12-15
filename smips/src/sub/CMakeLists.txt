cmake_minimum_required(VERSION 3.2)

set(SOURCES
		copy_ctor.cpp
		destructor.cpp
		solve.cpp
		solve2.cpp
		sub.cpp
		update.cpp)

add_library(sub ${SOURCES})
