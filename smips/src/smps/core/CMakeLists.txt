cmake_minimum_required(VERSION 3.2)

set(SOURCES
		colstate.cpp
		endatastate.cpp
		namestate.cpp
        nonestate.cpp
		rhsstate.cpp
        rowstate.cpp)

add_library(core ${SOURCES})
