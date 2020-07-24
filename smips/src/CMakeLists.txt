cmake_minimum_required(VERSION 3.2)

find_package(Gurobi REQUIRED)

include_directories(${GUROBI_INCLUDE_DIRS})

add_subdirectory(cutfamilies)
add_subdirectory(smps)


# TODO fix all this
set(SOURCES
        deterministicequivalent.cpp
        masterproblem.cpp
        problem.cpp
        subproblem.cpp)

add_library(libsmips ${SOURCES})
target_link_libraries(libsmips
        cutfamilies
        smps
        ${GUROBI_LIBRARIES}
        armadillo)

add_executable(smips main.cpp)
target_link_libraries(smips libsmips)
