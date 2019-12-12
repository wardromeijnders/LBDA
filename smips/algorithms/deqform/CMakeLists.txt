cmake_minimum_required(VERSION 3.2)

set(HEADERS
		deqform.h)

set(SOURCES
		deqform.cpp
		destructor.cpp
		init_fs.cpp
		init_ss.cpp
		solve.cpp)

add_library(deqform ${HEADERS} ${SOURCES})
