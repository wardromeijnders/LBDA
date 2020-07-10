cmake_minimum_required(VERSION 3.2)

find_package(Gurobi REQUIRED)

include_directories(${GUROBI_INCLUDE_DIRS})

add_subdirectory(cutfamilies)
add_subdirectory(smps)

set(SOURCES
        deterministicequivalent.cpp
        masterproblem.cpp
        subproblem.cpp
        problem.cpp)

add_executable(smips main.cpp ${SOURCES})

target_link_libraries(smips
        cutfamilies
        smps
        ${GUROBI_LIBRARIES}
        armadillo)
