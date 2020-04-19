include(FindPackageHandleStandardArgs)

set(COIN_OR_HOME "$ENV{COIN_OR_HOME}")

find_library(COIN_OR_SMI_LIBRARY
		NAMES libSmi.so
		PATHS "${COIN_OR_HOME}/lib")

set(COIN_OR_SMI_INCLUDES ${COIN_OR_HOME}/include)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(COIN_OR_SMI DEFAULT_MSG
		COIN_OR_SMI_INCLUDES
		COIN_OR_SMI_LIBRARY)

if(COIN_OR_SMI_FOUND)
	set(COIN_OR_SMI_INCLUDE_DIRS ${COIN_OR_HOME}/include/coin)

	set(COIN_OR_SMI_LIBRARIES ${COIN_OR_SMI_LIBRARY})

	mark_as_advanced(
			COIN_OR_SMI_LIBRARY
			COIN_OR_HOME)
else()
	message("-- Could not find COIN-OR SMI installation.")
endif()
