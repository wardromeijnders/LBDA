cmake_minimum_required(VERSION 3.2)

set(HEADERS
		ald.h)

set(SOURCES
		add_cut.cpp
		ald.cpp
		clean.cpp
		copy_ctor.cpp
		destructor.cpp
		gmi_cut.cpp
		is_integer.cpp
		solve.cpp
		update.cpp)

add_library(ald ${HEADERS} ${SOURCES})
