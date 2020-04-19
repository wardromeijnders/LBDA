cmake_minimum_required(VERSION 3.2)

find_package(Gurobi REQUIRED)
find_package(CoinOrSmi REQUIRED)

include_directories(${GUROBI_INCLUDE_DIRS})
include_directories(${COIN_OR_SMI_INCLUDES})

add_subdirectory(decompositions)
add_subdirectory(deterministicequivalent)
add_subdirectory(masterproblem)
add_subdirectory(problem)
add_subdirectory(subproblem)

add_executable(smips main.cpp)
target_link_libraries(smips
        decompositions
        deterministicequivalent
        problem
        master
        ${GUROBI_LIBRARIES}
        ${COIN_OR_SMI_LIBRARIES})
