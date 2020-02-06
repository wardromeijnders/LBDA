cmake_minimum_required(VERSION 3.2)

set(SOURCES
		append_minus_eye.cpp
		append_r.cpp
		append_zeros2.cpp
		data.cpp
		rand_gaus_mat.cpp
		rand_unif_mat.cpp
		rand_unif_vec.cpp
		)

add_library(data ${SOURCES})
