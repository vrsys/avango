################################################################
# Avango
################################################################
PROJECT(AVANGO CXX)

# version number
set(AVANGO_MAJOR 1)
set(AVANGO_MINOR 0)
set(AVANGO_PATCH 0)
set(AVANGO_VERSION ${AVANGO_MAJOR}.${AVANGO_MINOR}.${AVANGO_PATCH})
set(AVANGO_DESCRIPTION "AVANGO")
set(AVANGO_HOMEPAGE "https://github.com/vrsys")
set(AVANGO_EXENAME "AVANGO")
set(AVANGO_PACKAGENAME "AVANGO")

# We require at least version 2.8.0
cmake_minimum_required(VERSION 2.8.0)

if (UNIX)
	find_package(PkgConfig)
endif(UNIX)

# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

################################################################
# Configure and find libraries
################################################################
set(GLOBAL_EXT_DIR ${AVANGO_SOURCE_DIR}/externals)

MESSAGE(${GLOBAL_EXT_DIR})

if (UNIX)
	pkg_check_modules(GL REQUIRED gl)
endif (UNIX)

include(find_compiler)
include(find_boost)

set(LIBRARIES
    ${BOOST_LIBRARIES}
    ${GL_LIBRARIES}
)

set(LIB_PATHS
    ${GL_LIBRARY_DIRS}
	  ${BOOST_LIBRARY_DIRS}
)

set(INCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/build
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${BOOST_INCLUDE_DIRS}
    ${GL_INCLUDE_DIRS}
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

################################################################
# preprocessor configuration
################################################################
IF (UNIX)
	set(CMAKE_CXX_FLAGS_RELEASE "-s -O4 --std=c++0x -fpermissive")
	set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++0x -fpermissive")
ELSEIF(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "-D _SECURE_SCL=0 -D _SCL_SECURE_NO_WARNINGS -D _CRT_SECURE_NO_DEPRECATE /MD /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG /MDd /Zi")
ENDIF(UNIX)

################################################################
# Avango Configuration
################################################################
SET (AVANGO_VERSION_MAJOR "1" CACHE STRING "Major version of AvangoNG.")
SET (AVANGO_VERSION_MINOR "90" CACHE STRING "Minor version of AvangoNG.")
SET (AVANGO_VERSION_MAINT "0" CACHE STRING "Maintenance version of AvangoNG.")

SET (AVANGO_DAEMON_DEBUG "false" CACHE BOOL "Enable Daemon debugging")
SET (AVANGO_DAEMON_VRPN_SUPPORT "false" CACHE BOOL "Enable VRPN support")
SET (AVANGO_DISTRIBUTION_SUPPORT "false" CACHE BOOL "Set to enable distribution support.")
SET (AVANGO_ZMQ_DISTRIBUTION_SUPPORT "false" CACHE BOOL "Set to use ZMQ for distribution.")
SET (AVANGO_PCL_SUPPORT "true" CACHE BOOL "Use PCL.")
SET (AVANGO_LOG_LEVEL "WARN" CACHE STRING "Set AvangoNG log level: FATAL, ERROR, WARN, INFO, DEBUG, TRACE")
SET (AVANGO_UNITTESTS "false" CACHE BOOL "Compile Unittests for Avango.")

# enable daemon debugging
SET (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D _AVANGO_DAEMON_DEBUG")
SET (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D _AVANGO_DAEMON_VRPN_SUPPORT=${AVANGO_DAEMON_VRPN_SUPPORT}")
SET (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D _AVANGO_DISTRIBUTION_SUPPORT=${AVANGO_DISTRIBUTION_SUPPORT}")
SET (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D _AVANGO_ZMQ_DISTRIBUTION_SUPPORT=${AVANGO_ZMQ_DISTRIBUTION_SUPPORT}")
SET (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D _AVANGO_PCL_SUPPORT=${AVANGO_PCL_SUPPORT}")


# add DEBUG flag for debug built
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D DEFINE_AVANGO_DEBUG=1")

# set debug log level and version strings
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D _AVANGO_LOG_LEVEL=${AVANGO_LOG_LEVEL}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D _AVANGO_VERSION_MAJOR=${AVANGO_VERSION_MAJOR}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D _AVANGO_VERSION_MINOR=${AVANGO_VERSION_MINOR}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D _AVANGO_VERSION_MAINT=${AVANGO_VERSION_MAINT}")

IF (MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D AV_LIBRARY")
ENDIF (MSVC)


################################################################
# Create libraries
################################################################

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(avango-core)
add_subdirectory(avango-daemon)
#add_subdirectory(avango-python)

################################################################
# Summary
################################################################

message( "" )
message( "Summary:" )
message( " build type: ${CMAKE_BUILD_TYPE}" )
message( "" )
message( " boost:" )
message( "   library: ${BOOST_LIBRARIES}" )
message( "   library path: ${BOOST_LIBRARY_DIRS}" )
message( "   include: ${BOOST_INCLUDE_DIRS}" )
message( "" )
message( " gl:" )
message( "   library: ${GL_LIBRARIES}" )
message( "   include: ${GL_INCLUDE_DIRS}" )
message( "" )
