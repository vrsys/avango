################################################################
# Avango
################################################################
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
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
SET(GLOBAL_EXT_DIR ${AVANGO_SOURCE_DIR}/externals)

if (UNIX)
	pkg_check_modules(GL REQUIRED gl)
endif (UNIX)

include(find_compiler)

include(find_guacamole)
SET(GUACAMOLE_EXT_DIR ${GUACAMOLE_INCLUDE_DIRS}/../externals)

include(find_schism)
include(find_boost)
include(find_python)

set(LIBRARIES
    ${BOOST_LIBRARIES}
    ${GUACAMOLE_LIBRARIES}
    ${GL_LIBRARIES}
)

set(LIB_PATHS
	  ${BOOST_LIBRARY_DIRS}
    ${GUACAMOLE_LIBRARY_DIRS}
    ${GL_LIBRARY_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set(INCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/build
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${BOOST_INCLUDE_DIRS}
    ${GUACAMOLE_INCLUDE_DIRS}
    ${GL_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(ALL
                    ${INCLUDE_PATHS}
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
  add_definitions(-D AV_INSTANTIATE_FIELD_TEMPLATES -D NOMINMAX)
ENDIF(UNIX)

################################################################
# Avango Configuration
################################################################
SET (AVANGO_VERSION_MAJOR "1" CACHE STRING "Major version of AvangoNG.")
SET (AVANGO_VERSION_MINOR "90" CACHE STRING "Minor version of AvangoNG.")
SET (AVANGO_VERSION_MAINT "0" CACHE STRING "Maintenance version of AvangoNG.")

SET (AVANGO_DAEMON_VRPN_SUPPORT "false" CACHE BOOL "Enable using VRPN.")
SET (AVANGO_DAEMON_DEBUG "false" CACHE BOOL "Enable Daemon debugging")
SET (AVANGO_DAEMON_VRPN_SUPPORT "false" CACHE BOOL "Enable VRPN support")
SET (AVANGO_DISTRIBUTION_SUPPORT "false" CACHE BOOL "Set to enable distribution support.")
SET (AVANGO_ZMQ_DISTRIBUTION_SUPPORT "false" CACHE BOOL "Set to use ZMQ for distribution.")
SET (AVANGO_PCL_SUPPORT "false" CACHE BOOL "Use PCL.")
SET (AVANGO_LOG_LEVEL "WARN" CACHE STRING "Set AvangoNG log level: FATAL, ERROR, WARN, INFO, DEBUG, TRACE")
SET (AVANGO_UNITTESTS "false" CACHE BOOL "Compile Unittests for Avango.")

################################################################
# Create libraries
################################################################

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

ADD_SUBDIRECTORY(avango-unittest)
ADD_SUBDIRECTORY(avango-core)
ADD_SUBDIRECTORY(avango-daemon)
ADD_SUBDIRECTORY(avango-python)

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
message( " guacamole:" )
message( "   library: ${GUACAMOLE_LIBRARIES}" )
message( "   library path: ${GUACAMOLE_LIBRARY_DIRS}" )
message( "   include: ${GUACAMOLE_INCLUDE_DIRS}" )
message( "" )
message( " schism:" )
message( "   library: ${SCHISM_LIBRARIES}" )
message( "   library path: ${SCHISM_LIBRARY_DIRS}" )
message( "   include: ${SCHISM_INCLUDE_DIRS}" )
message( "" )
message( " python:" )
message( "   library: ${PYTHON_LIBRARIES}" )
message( "   library path: ${PYTHON_LIBRARY_DIRS}" )
message( "   include: ${PYTHON_INCLUDE_DIRS}" )
message( "" )
message( " gl:" )
message( "   library: ${GL_LIBRARIES}" )
message( "   include: ${GL_INCLUDE_DIRS}" )
message( "" )

