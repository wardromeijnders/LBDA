cmake_minimum_required(VERSION 3.2)

find_package(Gurobi REQUIRED)
include_directories(${GUROBI_INCLUDE_DIRS})

add_subdirectory(benders)
add_subdirectory(data)
add_subdirectory(decompositions)
add_subdirectory(deqform)
add_subdirectory(masterproblem)
add_subdirectory(problem)
add_subdirectory(subproblem)
