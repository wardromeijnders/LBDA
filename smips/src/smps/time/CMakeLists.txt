cmake_minimum_required(VERSION 3.2)

set(SOURCES
		endatastate.cpp
        nonestate.cpp
        periodstate.cpp
        timestate.cpp)

add_library(time ${SOURCES})
