cmake_minimum_required(VERSION 3.2)

find_package(Gurobi REQUIRED)
include_directories(${GUROBI_INCLUDE_DIRS})

set(cutFamiliesSources
        cutfamilies/cutfamily.cpp
        cutfamilies/loosebenders.cpp
        cutfamilies/lpdual.cpp
        cutfamilies/strongbenders.cpp)

set(smpsSources
        smps/coreparser.cpp
        smps/dataline.cpp
        smps/fileparser.cpp
        smps/smps.cpp
        smps/stochparser.cpp
        smps/timeparser.cpp)

set(lbdaSources
        deterministicequivalent.cpp
        masterproblem.cpp
        problemdata.cpp
        subproblem.cpp)

# (Internal) library
add_library(lbda
        ${cutFamiliesSources}
        ${smpsSources}
        ${lbdaSources})
target_link_libraries(lbda PRIVATE armadillo ${GUROBI_LIBRARIES})

# Command-line executable for the LBDA+ code
add_executable(lbdaexec main.cpp)
target_link_libraries(lbdaexec PRIVATE armadillo ${GUROBI_LIBRARIES} lbda)

# LBDA+ code, with Python bindings for use there
pybind11_add_module(lbdapy bindings.cpp)
target_link_libraries(lbdapy PRIVATE armadillo ${GUROBI_LIBRARIES} lbda)
