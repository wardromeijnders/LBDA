cmake_minimum_required(VERSION 3.2)

set(SOURCES
		addcut.cpp
		copyconstructor.cpp
		cuts.cpp
		destructor.cpp
		master.cpp
		n1.cpp
		solve.cpp
		xcoeffs.cpp)

add_library(master ${SOURCES})
