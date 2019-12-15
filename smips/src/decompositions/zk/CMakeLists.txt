cmake_minimum_required(VERSION 3.2)

set(HEADERS
		../../../include/decompositions/zk.h)

set(SOURCES
		add_cut.cpp
		compute_tab_row_x.cpp
		compute_tab_row_y.cpp
		copy_ctor.cpp
		destructor.cpp
		gmi_cut.cpp
		is_integer.cpp
		move_ctor.cpp
		optimize.cpp
		solve.cpp
		subgradient.cpp
		transform_cut.cpp
		update.cpp
		zk.cpp)

add_library(zk ${HEADERS} ${SOURCES})
