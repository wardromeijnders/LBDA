cmake_minimum_required(VERSION 3.2)

set(HEADERS
		master.h)

set(SOURCES
		add_ald_cut.cpp
		add_zk_cut.cpp
		addcut.cpp
		copy_ctor.cpp
		destructor.cpp
		master.cpp
		solve.cpp)

add_library(master ${HEADERS} ${SOURCES})
