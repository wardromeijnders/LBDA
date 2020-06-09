cmake_minimum_required(VERSION 3.2)

set(SOURCES
		clearsub.cpp
		destructor.cpp
		evaluate.cpp
		fromsmps.cpp
		initsub.cpp
		problem.cpp
		)

add_library(problem ${SOURCES})
