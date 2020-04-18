cmake_minimum_required(VERSION 3.2)

set(SOURCES
        decomposition.cpp solve.cpp)

add_library(decomposition ${HEADERS} ${SOURCES})
