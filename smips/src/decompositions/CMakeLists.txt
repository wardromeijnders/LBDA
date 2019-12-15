cmake_minimum_required(VERSION 3.2)

add_subdirectory(ald)
add_subdirectory(cglp)
add_subdirectory(gomory)
add_subdirectory(lagrangian)
add_subdirectory(pslp)
add_subdirectory(zk)

add_library(decompositions empty.cpp)
target_link_libraries(decompositions
		ald
		cglp
		gomory
		lagrangian
		pslp
		zk)
