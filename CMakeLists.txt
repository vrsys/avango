################################################################
# Avango
################################################################
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
project(AVANGO CXX)

# version number
set(AVANGO_VERSION_MAJOR 2)
set(AVANGO_VERSION_MINOR 11)
set(AVANGO_VERSION_MAINT 1)

set(AVANGO_VERSION
    ${AVANGO_VERSION_MAJOR}.${AVANGO_VERSION_MINOR}.${AVANGO_VERSION_MAINT})
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

if (UNIX)
  pkg_check_modules(GL REQUIRED gl)
endif (UNIX)

include(find_compiler)
include(file_copy_macro)

include(find_guacamole)
set(GUACAMOLE_EXT_DIR ${GUACAMOLE_INCLUDE_DIRS}/../externals)

include(find_assimp)
include(find_boost)
set (BULLET_ROOT "/opt/bullet/default" CACHE PATH
    "Set to your bullet install path.")
find_package(Bullet)
include(find_json)
include(find_python)
include(find_schism)

if (AVANGO_DISTRIBUTION_SUPPORT)
    include(FindZMQ)
endif (AVANGO_DISTRIBUTION_SUPPORT)

if (UNIX)
    list(APPEND BOOST_LIBRARIES boost_thread boost_signals boost_python)
endif (UNIX)

set(LIBRARIES
    ${BOOST_LIBRARIES}
    ${GUACAMOLE_LIBRARIES}
    ${GL_LIBRARIES}
    ${SCHISM_LIBRARIES}
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

if (AVANGO_DISTRIBUTION_SUPPORT)
    list(APPEND LIB_PATHS ${ZMQ_LIBRARY_DIRS})
    list(APPEND INCLUDE_PATHS ${ZMQ_INCLUDE_DIRS})
endif (AVANGO_DISTRIBUTION_SUPPORT)

include_directories(ALL
                    ${INCLUDE_PATHS}
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

################################################################
# preprocessor configuration
################################################################
if (UNIX)
    set(CMAKE_CXX_FLAGS_RELEASE "-s -O4 --std=c++0x")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++0x")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG -D _SECURE_SCL=0 -D _SCL_SECURE_NO_WARNINGS -D _CRT_SECURE_NO_DEPRECATE /MD /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG /MDd /Zi")
    add_definitions(-D AV_INSTANTIATE_FIELD_TEMPLATES -D NOMINMAX)
endif(UNIX)

################################################################
# Avango Configuration
################################################################
set (AVANGO_LOG_LEVEL "WARN" CACHE STRING
    "Set AvangoNG log level: FATAL, ERROR, WARN, INFO, DEBUG, TRACE")
set (AVANGO_INSTALL_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib CACHE PATH
    "Install path for avango.")

option (AVANGO_DEBUG "Set to enable avango debugging.")
option (AVANGO_DISTRIBUTION_SUPPORT "Set to enable distribution support.")
option (AVANGO_PBR_SUPPORT "Set to enable PBR (point based rendering) support.")
option (AVANGO_PCL_SUPPORT "Use PCL.")
option (AVANGO_UNITTESTS "Compile Unittests for Avango.")
option (AVANGO_OCULUS_RIFT_SUPPORT "Compile Oculus Rift module for Avango.")
option (AVANGO_OPENAL_SUPPORT "Enable building OpenAL sound module.")
option (AVANGO_DISPLAY_SUPPORT "Enable building display setup module.")

################################################################
# Create libraries
################################################################
file(MAKE_DIRECTORY ${AVANGO_INSTALL_DIRECTORY})
file(MAKE_DIRECTORY ${AVANGO_INSTALL_DIRECTORY}/python2.7)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(avango-unittest)
add_subdirectory(avango-core)
add_subdirectory(avango-python)
add_subdirectory(avango-gua)
add_subdirectory(avango-daemon)
add_subdirectory(avango-tools)
add_subdirectory(avango-sound)

if (AVANGO_OCULUS_RIFT_SUPPORT)
    add_subdirectory(avango-oculus)
endif(AVANGO_OCULUS_RIFT_SUPPORT)

if (AVANGO_OPENAL_SUPPORT)
    add_subdirectory(avango-openal)
endif(AVANGO_OPENAL_SUPPORT)

################################################################
# Summary
################################################################

message(STATUS "" )
message(STATUS "Summary:" )
message(STATUS " build type: ${CMAKE_BUILD_TYPE}" )
message(STATUS "" )
message(STATUS " boost:" )
message(STATUS "   library: ${BOOST_LIBRARIES}" )
message(STATUS "   library path: ${BOOST_LIBRARY_DIRS}" )
message(STATUS "   include: ${BOOST_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " guacamole:" )
message(STATUS "   library: ${GUACAMOLE_LIBRARIES}" )
message(STATUS "   library path: ${GUACAMOLE_LIBRARY_DIRS}" )
message(STATUS "   include: ${GUACAMOLE_INCLUDE_DIRS}" )
message(STATUS "" )

if ( AVANGO_OCULUS_RIFT_SUPPORT  )
  message(STATUS " guacamole-oculus:" )
  message(STATUS "   library: ${GUACAMOLE_OCULUS_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_OCULUS_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_OCULUS_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_OCULUS_RIFT_SUPPORT )

message(STATUS " schism:" )
message(STATUS "   library: ${SCHISM_LIBRARIES}" )
message(STATUS "   library path: ${SCHISM_LIBRARY_DIRS}" )
message(STATUS "   include: ${SCHISM_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " python:" )
message(STATUS "   library: ${PYTHON_LIBRARIES}" )
message(STATUS "   library path: ${PYTHON_LIBRARY_DIRS}" )
message(STATUS "   include: ${PYTHON_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " gl:" )
message(STATUS "   library: ${GL_LIBRARIES}" )
message(STATUS "   include: ${GL_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " bullet:" )
message(STATUS "   library: ${BULLET_LIBRARIES}" )
message(STATUS "   library path: ${BULLET_LIBRARY_DIRS}" )
message(STATUS "   include: ${BULLET_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " json:" )
message(STATUS "   library: ${JSON_LIBRARIES}" )
message(STATUS "   library path: ${JSON_LIBRARY_DIRS}" )
message(STATUS "   include: ${JSON_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " assimp:" )
message(STATUS "   library: ${ASSIMP_LIBRARIES}" )
message(STATUS "   library path: ${ASSIMP_LIBRARY_DIRS}" )
message(STATUS "   include: ${ASSIMP_INCLUDE_DIRS}" )
message(STATUS "" )

