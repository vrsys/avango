##############################################################################
# search paths
##############################################################################
SET(ZMQ_INCLUDE_SEARCH_DIRS
  ${GLOBAL_EXT_DIR}/zeromq-2.2.0/include
  ${ZMQ_INCLUDE_DIRS}
  ${ZMQ_INCLUDE_SEARCH_DIR}
  "/opt/zmq/current/include"
)

SET(ZMQ_LIBRARY_SEARCH_DIRS
  ${GLOBAL_EXT_DIR}/zeromq-2.2.0/lib
  ${ZMQ_LIBRARY_DIRS}
  ${ZMQ_LIBRARY_SEARCH_DIR}
  "/opt/zmq/current/lib"
)

##############################################################################
# search
##############################################################################
find_path (ZMQ_INCLUDE_DIRS
           NAMES zmq.h
		       PATHS
		       ${ZMQ_INCLUDE_SEARCH_DIRS}
)

find_library (ZMQ_LIBRARIES
              NAMES libzmq.so libzmq.lib
			  PATH_SUFFIXES release debug 
			  PATHS ${ZMQ_LIBRARY_SEARCH_DIRS})

##############################################################################
# verify
##############################################################################
if (ZMQ_INCLUDE_DIRS AND ZMQ_LIBRARIES)
  MESSAGE(STATUS "-- Found ZMQ ${ZMQ_LIBRARIES}")
endif()
