cmake_minimum_required(VERSION 3.2)

set(SOURCES
		#classic_ri.cpp
		clearsub.cpp
		destructor.cpp
		evaluate.cpp
		initsub.cpp
		problem.cpp
		problem2.cpp
		randominstance.cpp
		setbounds.cpp
		setgaussianomega.cpp
		#sizes.cpp
		#sslp.cpp
		ssv95.cpp)

add_library(problem ${SOURCES})
