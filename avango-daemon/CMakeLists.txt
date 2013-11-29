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
    STRING(REPLACE ${_CUR_VARIABLE} _${_STRIPPED_VARIABLE} AVANGO_CONFIG_OUT ${AVANGO_CONFIG_OUT})
ENDFOREACH(_CUR_VARIABLE)

FILE(WRITE include/avango/daemon/Config.h ${AVANGO_CONFIG_OUT})

###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_DAEMON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/daemon/Device.cpp
    src/avango/daemon/DeviceActuator.cpp
    src/avango/daemon/DeviceDaemon.cpp
    src/avango/daemon/DeviceSensor.cpp
    src/avango/daemon/DeviceService.cpp
    src/avango/daemon/DTrack.cpp
    src/avango/daemon/Init.cpp
    src/avango/daemon/LinuxEvent.cpp
    src/avango/daemon/Station.cpp
    src/avango/daemon/StationBlock.cpp
    src/avango/daemon/StationSegment.cpp
    src/avango/daemon/dtrack/dtrack.cpp
    include/avango/daemon/Config.h
    include/avango/daemon/Device.h
    include/avango/daemon/DeviceActuator.h
    include/avango/daemon/DeviceDaemon.h
    include/avango/daemon/DeviceSensor.h
    include/avango/daemon/DeviceService.h
    include/avango/daemon/DTrack.h
    include/avango/daemon/Doxygen.h
    include/avango/daemon/EventMapping.win.h
    include/avango/daemon/HIDInput.h
    include/avango/daemon/Init.h
    include/avango/daemon/LinuxEvent.h
    include/avango/daemon/LinuxInput.win.h
    include/avango/daemon/SharedMemorySegment.h
    include/avango/daemon/Station.h
    include/avango/daemon/StationBlock.h
    include/avango/daemon/StationSegment.h
    include/avango/daemon/WacomTablet.h
    include/avango/daemon/Wiimote.h
    include/avango/daemon/WiimoteActuator.h
    src/avango/daemon/dtrack/dtrack.h
)

IF (UNIX)
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/HIDInput.linux.cpp")
        LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/SharedMemorySegment.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/WacomTablet.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/Wiimote.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/WiimoteActuator.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/HIDInput.linux.h")

ELSEIF (MSVC)
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/EventMapping.win.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/HIDInput.win.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/HIDInput.win.h")
ENDIF (UNIX)

IF (DEFINE_AVANGO_DAEMON_VRPN_SUPPORT)
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/VRPNClient.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/VRPNClient.h")
ENDIF (DEFINE_AVANGO_DAEMON_VRPN_SUPPORT)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${PYTHON_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
)

ADD_LIBRARY(avango_daemon SHARED
    ${AVANGO_DAEMON_SRC}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ${PYTHON_INCLUDE_DIRS}
    ../avango-gua/include
)

ADD_DEPENDENCIES ( avango_daemon avango)

###############################################################################
# set preprocessor configuration
###############################################################################
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_DEBUG=${AVANGO_DAEMON_DEBUG}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_VRPN_SUPPORT=${AVANGO_DAEMON_VRPN_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DISTRIBUTION_SUPPORT=${AVANGO_DISTRIBUTION_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_ZMQ_DISTRIBUTION_SUPPORT=${AVANGO_ZMQ_DISTRIBUTION_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_PCL_SUPPORT=${AVANGO_PCL_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_LOG_LEVEL=${AVANGO_LOG_LEVEL}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MAJOR=${AVANGO_MAJOR}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MINOR=${AVANGO_MINOR}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MAINT=${AVANGO_MAINT}")

IF (MSVC)
    SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_DAEMON_LIBRARY")
ENDIF (MSVC)

set_target_properties(avango_daemon PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})

TARGET_LINK_LIBRARIES( avango_daemon
  debug avango_gua avango_script avango ${LIBRARIES}
  optimized avango_gua avango_script avango ${LIBRARIES})

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_daemon ${AVANGO_INSTALL_DIRECTORY}/avango/avango_daemon.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_daemon ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_daemon.so)
ENDIF (MSVC)

