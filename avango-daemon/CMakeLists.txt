###############################################################################
# generate Config.h
###############################################################################
FILE(READ include/avango/daemon/Config.h.in AVANGO_CONFIG_IN)

SET(AVANGO_CONFIG_OUT ${AVANGO_CONFIG_IN})

STRING(REGEX MATCHALL "%\\([^\\)]+\\)s" AVANGO_CONFIG_VARIABLES ${AVANGO_CONFIG_IN})

FOREACH(_CUR_VARIABLE ${AVANGO_CONFIG_VARIABLES})
	SET(_STRIPPED_VARIABLE "")
	STRING(REGEX REPLACE "%\\(" "" _STRIPPED_VARIABLE ${_CUR_VARIABLE})
	STRING(REGEX REPLACE "\\)s" "" _STRIPPED_VARIABLE ${_STRIPPED_VARIABLE})
	STRING(REPLACE ${_CUR_VARIABLE} ${_STRIPPED_VARIABLE} AVANGO_CONFIG_OUT ${AVANGO_CONFIG_OUT})		
ENDFOREACH(_CUR_VARIABLE)

FILE(WRITE include/avango/daemon/Config.h ${AVANGO_CONFIG_OUT})

###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_DAEMON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/daemon/*.cpp
    src/avango/daemon/dtrack/*.cpp
	  src/avango/daemon/dtrack/*.h
    include/avango/daemon/*.h
)

IF (NOT DEFINE_AVANGO_DAEMON_VRPN_SUPPORT)
    LIST(REMOVE_ITEM AVANGO_DAEMON_SRC "src/avango/daemon/VRPNClient.cpp")
ENDIF (NOT DEFINE_AVANGO_DAEMON_VRPN_SUPPORT)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(${LIB_PATHS})

ADD_LIBRARY( avango_daemon SHARED
    	     ${AVANGO_DAEMON_SRC}
)

INCLUDE_DIRECTORIES( avango_daemon 
										${INCLUDE_PATHS} 
										include
										../avango-core/include)

ADD_DEPENDENCIES ( avango_daemon avango_core)

IF (NOT ${LIBRARIES} STREQUAL "")
    TARGET_LINK_LIBRARIES( avango_daemon debug ${LIBRARIES} optimized ${LIBRARIES})
ENDIF (NOT ${LIBRARIES} STREQUAL "")


