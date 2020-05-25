cmake_minimum_required(VERSION 3.2)

set(SOURCES
		endatastate.cpp
        nonestate.cpp
		stochstate.cpp)

add_library(stoch ${SOURCES})
