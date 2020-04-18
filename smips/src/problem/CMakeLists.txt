cmake_minimum_required(VERSION 3.2)

set(SOURCES
		clearsub.cpp
		destructor.cpp
		evaluate.cpp
		initsub.cpp
		problem.cpp
		ssv95.cpp)

add_library(problem ${SOURCES})
