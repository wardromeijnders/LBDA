cmake_minimum_required(VERSION 3.2)

add_library(smps
        coreparser.cpp
        dataline.cpp
        fileparser.cpp
        smps.cpp
        stochparser.cpp
        timeparser.cpp)

target_link_libraries(smps)
