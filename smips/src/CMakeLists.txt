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

set(smipsSources
        deterministicequivalent.cpp
        masterproblem.cpp
        problemdata.cpp
        subproblem.cpp)

# (Internal) library
add_library(smips
        ${cutFamiliesSources}
        ${smpsSources}
        ${smipsSources})
target_link_libraries(smips PRIVATE armadillo ${GUROBI_LIBRARIES})

# Command-line executable for the SMIPS code
add_executable(smipsexec main.cpp)
target_link_libraries(smipsexec PRIVATE armadillo ${GUROBI_LIBRARIES} smips)

# SMIPS code, with Python bindings for use there
pybind11_add_module(smipspy bindings.cpp)
target_link_libraries(smipspy PRIVATE armadillo ${GUROBI_LIBRARIES} smips)
