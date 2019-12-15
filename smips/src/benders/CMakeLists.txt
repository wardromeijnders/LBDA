cmake_minimum_required(VERSION 3.2)

set(SOURCES
		ald_affine.cpp
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

add_library(benders ${HEADERS} ${SOURCES})
target_link_libraries(benders
		decompositions
		master
		sub)
