cmake_minimum_required(VERSION 3.2)

set(HEADERS
		../../../include/decompositions/gomory.h)

set(SOURCES
		copyconstructor.cpp
		destructor.cpp
		gomory.cpp
		settimelimit.cpp
		update.cpp)

add_library(gomory ${HEADERS} ${SOURCES})
target_link_libraries(gomory relaxation)
