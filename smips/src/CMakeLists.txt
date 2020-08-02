cmake_minimum_required(VERSION 3.2)

find_package(Gurobi REQUIRED)

include_directories(${GUROBI_INCLUDE_DIRS})

add_library(deterministicequivalent deterministicequivalent.cpp)
target_link_libraries(deterministicequivalent
        problem
        ${GUROBI_LIBRARIES}
        armadillo)

add_library(masterproblem masterproblem.cpp)
target_link_libraries(masterproblem
        problem
        ${GUROBI_LIBRARIES}
        armadillo
        cutfamilies)

add_library(problem problem.cpp)
target_link_libraries(problem armadillo)

add_library(subproblem subproblem.cpp)
target_link_libraries(subproblem
        problem
        ${GUROBI_LIBRARIES}
        armadillo)

add_subdirectory(cutfamilies)
add_subdirectory(smps)

# Master library, contains all SMIPS code
add_library(libsmips SHARED dummy.cpp)
target_link_libraries(libsmips
        deterministicequivalent
        masterproblem
        problem
        subproblem
        cutfamilies
        smps)

add_executable(smips main.cpp)
target_link_libraries(smips libsmips)
