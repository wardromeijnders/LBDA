cmake_minimum_required(VERSION 3.2)

add_subdirectory(ald)
add_subdirectory(cglp)
add_subdirectory(gomory)
add_subdirectory(lagrangian)
add_subdirectory(master)
add_subdirectory(pslp)
add_subdirectory(sub)
add_subdirectory(zk)

set(HEADERS
		benders.h)

set(SOURCES
		ald_affine.dep
		ald_cut.cpp
		ald_solve.cpp
		benders.cpp
		compute_gomory.cpp
		computeTx.cpp
		copy_ctor.cpp
		destructor.cpp
		lbda.cpp
		lbdacut.cpp
		lpcut.cpp
		lpsolve.cpp
		sb_cut.cpp
		strong_benders.cpp
		zk_solve.cpp)

add_library(decomposition ${HEADERS} ${SOURCES})
target_link_libraries(decomposition
		ald
		cglp
		gomory
		lagrangian
		master
		pslp
		sub
		zk)
