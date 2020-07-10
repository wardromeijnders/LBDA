cmake_minimum_required(VERSION 3.2)

set(SOURCES
        cutfamily.cpp
		loosebenders.cpp
		lpdual.cpp
		strongbenders.cpp)

add_library(cutfamilies ${SOURCES})
