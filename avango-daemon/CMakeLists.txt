set (AVANGO_DAEMON_VERSION_MAJOR "${AVANGO_VERSION_MAJOR}")
set (AVANGO_DAEMON_VERSION_MINOR "${AVANGO_VERSION_MINOR}")
set (AVANGO_DAEMON_VERSION_MAINT "${AVANGO_VERSION_MAINT}")
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
    src/avango/daemon/KinectTrack.cpp
    src/avango/daemon/Init.cpp
    src/avango/daemon/LinuxEvent.cpp
    src/avango/daemon/Station.cpp
    src/avango/daemon/StationBlock.cpp
    src/avango/daemon/StationSegment.cpp
    src/avango/daemon/dtrack/dtrack.cpp
    src/avango/daemon/tuio/TUIO/TuioClient.cpp
    src/avango/daemon/tuio/TUIO/TuioContainer.h
    src/avango/daemon/tuio/TUIO/TuioCursor.h
    src/avango/daemon/tuio/TUIO/TuioFinger.h
    src/avango/daemon/tuio/TUIO/TuioHand.h
    src/avango/daemon/tuio/TUIO/TuioListener.h
    src/avango/daemon/tuio/TUIO/TuioObject.h
    src/avango/daemon/tuio/TUIO/TuioPoint.h
    src/avango/daemon/tuio/TUIO/TuioServer.cpp
    src/avango/daemon/tuio/TUIO/TuioServer.h
    src/avango/daemon/tuio/TUIO/TuioTime.cpp
    src/avango/daemon/tuio/TUIO/TuioTime.h
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
    include/avango/daemon/KinectTrack.h
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

if (UNIX)
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/HIDInput.linux.cpp")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/SharedMemorySegment.cpp")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/WacomTablet.cpp")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/Wiimote.cpp")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/WiimoteActuator.cpp")
    list(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/HIDInput.linux.h")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/tuio/oscpack/ip/posix/NetworkingUtils.cpp")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/tuio/oscpack/ip/posix/UdpSocket.cpp")
elseif (MSVC)
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/EventMapping.win.cpp")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/HIDInput.win.cpp")
    list(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/HIDInput.win.h")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/tuio/oscpack/ip/win32/NetworkingUtils.cpp")
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/tuio/oscpack/ip/win32/UdpSocket.cpp")
endif (UNIX)

if (DEFINE_AVANGO_DAEMON_VRPN_SUPPORT)
    list(APPEND AVANGO_DAEMON_SRC "src/avango/daemon/VRPNClient.cpp")
    list(APPEND AVANGO_DAEMON_SRC "include/avango/daemon/VRPNClient.h")
endif (DEFINE_AVANGO_DAEMON_VRPN_SUPPORT)

###############################################################################
# set link directories and link dependencies
###############################################################################

link_directories(
    ${LIB_PATHS}
    ${PYTHON3_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
    ${ZMQ_LIBRARY_DIRS}
)

include_directories(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${ZMQ_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ${PYTHON3_INCLUDE_DIRS}
    ../avango-gua/include
    src/avango/daemon/tuio/TUIO
    src/avango/daemon/tuio/oscpack
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${RPC_INCLUDE_DIR} ${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY(avango_daemon SHARED
    ${AVANGO_DAEMON_SRC}
)

add_dependencies ( avango_daemon avango)

###############################################################################
# set preprocessor configuration
###############################################################################
set (AVANGO_DAEMON_BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MAJOR=${AVANGO_VERSION_MAJOR}")
set (AVANGO_DAEMON_BUILD_FLAGS "${AVANGO_DAEMON_BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MINOR=${AVANGO_VERSION_MINOR}")
set (AVANGO_DAEMON_BUILD_FLAGS "${AVANGO_DAEMON_BUILD_FLAGS} -D _AVANGO_DAEMON_VERSION_MAINT=${AVANGO_VERSION_MAINT}")

if (MSVC)
    set(AVANGO_DAEMON_BUILD_FLAGS "${AVANGO_DAEMON_BUILD_FLAGS} -D AV_DAEMON_LIBRARY")
endif (MSVC)

set_target_properties(avango_daemon PROPERTIES COMPILE_FLAGS ${AVANGO_DAEMON_BUILD_FLAGS})

target_link_libraries( avango_daemon
  avango_gua 
  avango_script 
  avango 
  ${ZMQ_LIBRARIES}
  )

if (MSVC)
  target_link_libraries(avango_daemon winmm.lib Ws2_32.lib)
endif (MSVC)

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
if (MSVC)
  post_build_install_target(avango_daemon ${AVANGO_INSTALL_DIRECTORY}/avango/avango_daemon.dll)
elseif (UNIX)
  post_build_install_target(avango_daemon ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_daemon.so)
endif (MSVC)

