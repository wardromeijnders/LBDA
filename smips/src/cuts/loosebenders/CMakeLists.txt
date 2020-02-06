cmake_minimum_required(VERSION 3.2)

set(SOURCES
		computecut.cpp
		computegomory.cpp
		loosebenders.cpp)

add_library(loosebenders ${HEADERS} ${SOURCES})
target_link_libraries(loosebenders
		sub
		gomory)