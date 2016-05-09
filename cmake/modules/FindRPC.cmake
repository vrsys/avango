##############################################################################
# search paths
##############################################################################
SET(RPC_SEARCH_PATHS
	${GLOBAL_EXT_DIR}/rpc/include
	${GLOBAL_EXT_DIR}/rpc/lib
	/usr/include
)

##############################################################################
# search
##############################################################################
find_path (RPC_INCLUDE_DIR
           NAMES rpc/rpc.h
		       PATHS
		       ${RPC_SEARCH_PATHS}
)

find_library (RPC_LIBRARY
              NAMES rpc oncrpc
			  PATH_SUFFIXES release debug 
			  PATHS ${RPC_SEARCH_PATHS})

if (RPC_INCLUDE_DIR AND RPC_LIBRARY)
  MESSAGE(STATUS "-- Found RPC ${RPC_LIBRARY}")
endif()