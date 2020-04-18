cmake_minimum_required(VERSION 3.2)

set(SOURCES
		deterministicequivalent.cpp
		destructor.cpp
		initfirststage.cpp
		initsecondstage.cpp
		solve.cpp)

add_library(deterministicequivalent ${SOURCES})
