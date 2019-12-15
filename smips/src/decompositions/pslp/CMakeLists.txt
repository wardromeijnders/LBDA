cmake_minimum_required(VERSION 3.2)

set(HEADERS
		../../../include/decompositions/pslp.h)

set(SOURCES
		copy_ctor.cpp
		l_shaped.cpp
		pslp.cpp
		strong_zk_cut.cpp
		update_rhs.cpp
		update_tau.cpp
		zk_cut.cpp)

add_library(pslp ${HEADERS} ${SOURCES})
