cmake_minimum_required(VERSION 3.2)

project(smips
		VERSION 1.0
		LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-Wall -Wextra)

if(CMAKE_BUILD_TYPE MATCHES RELEASE)
	add_definitions(-O3)
else()
	add_definitions(-O1)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
