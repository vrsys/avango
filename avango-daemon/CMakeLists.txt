SET (AVANGO_DAEMON_VERSION_MAJOR "${AVANGO_VERSION_MAJOR}")
SET (AVANGO_DAEMON_VERSION_MINOR "${AVANGO_VERSION_MINOR}")
SET (AVANGO_DAEMON_VERSION_MAINT "${AVANGO_VERSION_MAINT}")
option (AVANGO_DAEMON_DEBUG "Enable Daemon debugging")
option (AVANGO_DAEMON_VRPN_SUPPORT "Enable using VRPN.")

###############################################################################
# generate Config.h
###############################################################################

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/daemon/Config.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/daemon/Config.h )

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
    src/avango/daemon/tuio/TUIO/TuioClient.cpp
    src/avango/daemon/tuio/TUIO/TuioServer.cpp
    src/avango/daemon/tuio/TUIO/TuioTime.cpp
    src/avango/daemon/tuio/oscpack/osc/OscTypes.cpp
    src/avango/daemon/tuio/oscpack/osc/OscOutboundPacketStream.cpp
    src/avango/daemon/tuio/oscpack/osc/OscReceivedElements.cpp
    src/avango/daemon/tuio/oscpack/osc/OscPrintReceivedElements.cpp
    src/avango/daemon/TUIOInput.cpp
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
    include/avango/daemon/TUIOInput.h
    include/avango/daemon/windows_specific_daemon.h
    src/avango/daemon/dtrack/dtrack.h
)

IF (UNIX)
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/HIDInput.linux.cpp")
        LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/SharedMemorySegment.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/WacomTablet.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/Wiimote.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/WiimoteActuator.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/HIDInput.linux.h")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/tuio/oscpack/ip/posix/NetworkingUtils.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/tuio/oscpack/ip/posix/UdpSocket.cpp")

ELSEIF (MSVC)
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/EventMapping.win.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/HIDInput.win.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/HIDInput.win.h")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/tuio/oscpack/ip/win32/NetworkingUtils.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/tuio/oscpack/ip/win32/UdpSocket.cpp")
ENDIF (UNIX)

IF (DEFINE_AVANGO_DAEMON_VRPN_SUPPORT)
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/VRPNClient.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/VRPNClient.h")
ENDIF (DEFINE_AVANGO_DAEMON_VRPN_SUPPORT)

IF (${AVANGO_OCULUS_RIFT_SUPPORT})
    LIST(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/Oculus.cpp")
    LIST(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/Oculus.h")
ENDIF ()

include(find_ovr)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${PYTHON_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
    ${OVR_LIBRARY_DIRS}
)

ADD_LIBRARY(avango_daemon SHARED
    ${AVANGO_DAEMON_SRC}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${OVR_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ${PYTHON_INCLUDE_DIRS}
    ../avango-gua/include
    src/avango/daemon/tuio/TUIO
    src/avango/daemon/tuio/oscpack
)

ADD_DEPENDENCIES ( avango_daemon avango)

###############################################################################
# set preprocessor configuration
###############################################################################
SET (AVANGO_DAEMON_BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MAJOR=${AVANGO_MAJOR}")
SET (AVANGO_DAEMON_BUILD_FLAGS "${AVANGO_DAEMON_BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MINOR=${AVANGO_MINOR}")
SET (AVANGO_DAEMON_BUILD_FLAGS "${AVANGO_DAEMON_BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MAINT=${AVANGO_PATCH}")

IF (MSVC)
    SET(AVANGO_DAEMON_BUILD_FLAGS "${AVANGO_DAEMON_BUILD_FLAGS} -D AV_DAEMON_LIBRARY")
ENDIF (MSVC)

set_target_properties(avango_daemon PROPERTIES COMPILE_FLAGS ${AVANGO_DAEMON_BUILD_FLAGS})

LIST(APPEND LIBRARIES avango avango_script avango_gua)

IF (${AVANGO_OCULUS_RIFT_SUPPORT})
IF (UNIX)
  LIST(APPEND LIBRARIES udev Xinerama ${OVR_LIBRARIES} )
ELSEIF (MSVC)
  LIST(APPEND LIBRARIES winmm.lib guacamole ${OVR_LIBRARIES})
ENDIF(UNIX)
ENDIF()

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

