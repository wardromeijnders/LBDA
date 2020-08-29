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

# Master library, contains all SMIPS code
add_library(smips SHARED
        ${cutFamiliesSources}
        ${smpsSources}
        ${smipsSources})

target_link_libraries(smips
        ${GUROBI_LIBRARIES}
        armadillo)

add_executable(smipsexec main.cpp)
target_link_libraries(smipsexec smips)

pybind11_add_module(smipspy
		bindings.cpp
		${cutFamiliesSources}
        ${smpsSources}
        ${smipsSources})

target_link_libraries(smipspy PRIVATE armadillo ${GUROBI_LIBRARIES})
