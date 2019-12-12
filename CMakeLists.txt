cmake_minimum_required(VERSION 3.2)

project(smips
		VERSION 1.0
		LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-Wall)

if(CMAKE_BUILD_TYPE MATCHES RELEASE)
	add_definitions(-O3)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_library(GUROBI_C_LIBRARY
		NAMES libgurobi90.so
		PATHS "$ENV{GUROBI_HOME}/lib")

find_library(GUROBI_CXX_LIBRARY
		NAMES libgurobi_c++.a
		PATHS "$ENV{GUROBI_HOME}/lib")

include_directories("$ENV{GUROBI_HOME}/include")

add_subdirectory(smips)

add_executable(smips main.cpp)

target_link_libraries(smips
		decomposition
		deqform
		problem_data
		${GUROBI_C_LIBRARY}
		${GUROBI_CXX_LIBRARY})
