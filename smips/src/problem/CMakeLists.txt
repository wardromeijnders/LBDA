cmake_minimum_required(VERSION 3.2)

set(SOURCES
		classic_ri.cpp
		clear_sub.cpp
		../data/data.cpp
		destructor.cpp
		enforce_ccr.cpp
		evaluate.cpp
		init_sub.cpp
		problem.cpp
		problem2.cpp
		randominstance.cpp
		set_bounds.cpp
		set_omega_gaus.cpp
		sizes.cpp
		sslp.cpp
		ssv98.cpp)

add_library(problem ${SOURCES})
