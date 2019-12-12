cmake_minimum_required(VERSION 3.2)

set(HEADERS
		data.h
		problem.h)

set(SOURCES
		append_minus_eye.cpp
		append_r.cpp
		append_zeros2.cpp
		classic_ri.cpp
		clear_sub.cpp
		data.cpp
		destructor.cpp
		enforce_ccr.cpp
		evaluate.cpp
		init_sub.cpp
		problem.cpp
		problem2.cpp
		rand_gaus_mat.cpp
		rand_unif_mat.cpp
		rand_unif_vec.cpp
		randominstance.cpp
		set_bounds.cpp
		set_omega_gaus.cpp
		sizes.cpp
		sslp.cpp
		ssv98.cpp
		unif_real_vec.cpp)

add_library(problem_data ${HEADERS} ${SOURCES})
