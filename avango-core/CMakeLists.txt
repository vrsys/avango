# read replacement text from file
FILE(READ include/avango/Config.h.in AVANGO_CONFIG_IN)

###############################################################################
# write Config.h
###############################################################################
SET(AVANGO_CONFIG_OUT ${AVANGO_CONFIG_IN})

STRING(REGEX MATCHALL "%\\([^\\)]+\\)s" AVANGO_CONFIG_VARIABLES ${AVANGO_CONFIG_IN})

FOREACH(_CUR_VARIABLE ${AVANGO_CONFIG_VARIABLES})
	  SET(_STRIPPED_VARIABLE "")
	  STRING(REGEX REPLACE "%\\(" "" _STRIPPED_VARIABLE ${_CUR_VARIABLE})
	  STRING(REGEX REPLACE "\\)s" "" _STRIPPED_VARIABLE ${_STRIPPED_VARIABLE})
	  STRING(REPLACE ${_CUR_VARIABLE} _${_STRIPPED_VARIABLE} AVANGO_CONFIG_OUT ${AVANGO_CONFIG_OUT})		
ENDFOREACH(_CUR_VARIABLE)

STRING(REPLACE "av::logging::_AVANGO_LOG_LEVEL" "av::logging::${AVANGO_LOG_LEVEL}" AVANGO_CONFIG_OUT ${AVANGO_CONFIG_OUT})

FILE(WRITE include/avango/Config.h ${AVANGO_CONFIG_OUT})

###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_CORE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/*.cpp
  src/avango/actions/*.cpp
  src/avango/fields/*.cpp
  src/avango/interface/*.cpp
  src/avango/logging/*.cpp
  src/avango/nodes/*.cpp
  src/avango/streams/*.cpp
  src/avango/support/*.cpp
  src/avango/types/*.cpp
  include/avango/*.h
  include/avango/logging/*.h
)

###############################################################################
# optional sources for distribution support
###############################################################################
IF (${AVANGO_DISTRIBUTION_SUPPORT})
  file(GLOB AVANGO_CORE_DISTRIBUTION_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	src/avango/network/*.cpp
	src/avango/network/*.h
  )
  IF (NOT ${AVANGO_ZMQ_DISTRIBUTION_SUPPORT})
    list (REMOVE_ITEM  AVANGO_CORE_DISTRIBUTION_SRC
	src/avango/network/NetNodeClient.cpp
	src/avango/network/NetNodeClient.h
	src/avango/network/NetNodeServer.cpp
	src/avango/network/NetNodeServer.h
   )
  ENDIF (NOT ${AVANGO_ZMQ_DISTRIBUTION_SUPPORT})
ENDIF (${AVANGO_DISTRIBUTION_SUPPORT})

LINK_DIRECTORIES(${LIB_PATHS})

ADD_LIBRARY( avango SHARED
    	     ${AVANGO_CORE_SRC}
)

INCLUDE_DIRECTORIES( ${INCLUDE_PATHS} include)

###############################################################################
# set preprocessor configuration
###############################################################################
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_DEBUG=${AVANGO_DAEMON_DEBUG}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_DEBUG=${AVANGO_DAEMON_DEBUG}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_VRPN_SUPPORT=${AVANGO_DAEMON_VRPN_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DISTRIBUTION_SUPPORT=${AVANGO_DISTRIBUTION_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_ZMQ_DISTRIBUTION_SUPPORT=${AVANGO_ZMQ_DISTRIBUTION_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_PCL_SUPPORT=${AVANGO_PCL_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_LOG_LEVEL=${AVANGO_LOG_LEVEL}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_VERSION_MAJOR=${AVANGO_VERSION_MAJOR}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_VERSION_MINOR=${AVANGO_VERSION_MINOR}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_VERSION_MAINT=${AVANGO_VERSION_MAINT}" CACHE STRING "Build flags for avango.")

SET (AVANGO_BUILD_FLAGS ${BUILD_FLAGS})

IF (MSVC)
  SET(AVANGO_BUILD_FLAGS "${AVANGO_BUILD_FLAGS} -D AV_LIBRARY")
ENDIF (MSVC)

set_target_properties( avango PROPERTIES COMPILE_FLAGS ${AVANGO_BUILD_FLAGS})
set_target_properties( avango PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

TARGET_LINK_LIBRARIES( avango debug ${LIBRARIES} optimized ${LIBRARIES} )

###############################################################################
# targets for unittesting
###############################################################################
IF (AVANGO_UNITTESTS)
  add_subdirectory(src/avango/nodes/tests)
  add_subdirectory(src/avango/interface/tests)
  add_subdirectory(src/avango/types/tests)
  add_subdirectory(src/avango/fields/tests)
  add_subdirectory(src/avango/network/tests)
  add_subdirectory(src/avango/logging/tests)
ENDIF (AVANGO_UNITTESTS)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango ${AVANGO_INSTALL_DIRECTORY}/avango/avango.dll)
ELSEIF (UNIX)
  post_build_install_target(avango ${AVANGO_INSTALL_DIRECTORY}/avango/libavango.so)
ENDIF (MSVC)