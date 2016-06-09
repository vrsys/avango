################################################################
# Avango
################################################################
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
project(AVANGO CXX)

# version number
set(AVANGO_VERSION_MAJOR 2)
set(AVANGO_VERSION_MINOR 11)
set(AVANGO_VERSION_MAINT 11)

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

#if (AVANGO_PBR_SUPPORT)
#  include(FindGuacamolePLOD)
#endif (AVANGO_PBR_SUPPORT)

set(GUACAMOLE_EXT_DIR ${GUACAMOLE_INCLUDE_DIRS}/../externals)

if (UNIX)
  pkg_check_modules(JSON REQUIRED jsoncpp)
  pkg_check_modules(PYTHON3 REQUIRED python-3.4)
elseif (WIN32)
  include(findAssimp)
  include(findJson)
  include(findPythonLibs)
endif (UNIX)

if (UNIX)
  set (BOOST_ROOT "/opt/boost/latest" CACHE PATH "Set to your boost install")
elseif (MSVC)
  set (BOOST_ROOT ${GLOBAL_EXT_DIR}/boost CACHE PATH "Set to your boost install")
  message(STATUS "Set BOOST_ROOT to ${BOOST_ROOT}") 
  set (Boost_USE_MULTITHREADED ON)
  #set (Boost_USE_STATIC_LIBS ON)
  ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")
  find_package(Boost COMPONENTS python python3 REQUIRED)
endif(UNIX)

find_package(Boost COMPONENTS
  python3
  thread
  signals
  system
  REQUIRED
)

set (BULLET_ROOT 
     /opt/bullet/default
     ${GLOBAL_EXT_DIR}/bullet
     CACHE PATH
    "Set to your bullet install path.")
find_package(Bullet)

include(FindSchism)

include(FindZMQ)

if (${AVANGO_OCULUS_RIFT_SUPPORT})
  include(findOVR)
endif(${AVANGO_OCULUS_RIFT_SUPPORT})

set(LIBRARIES
    ${GUACAMOLE_LIBRARIES}
    ${GUACAMOLE_GUI_LIBRARIES}
    ${GUACAMOLE_VOLUME_LIBRARIES}
    ${GUACAMOLE_PLOD_LIBRARIES}
    ${GL_LIBRARIES}
    ${SCHISM_LIBRARIES}
)

set(LIB_PATHS
  ${Boost_LIBRARY_DIRS}
    ${GUACAMOLE_LIBRARY_DIRS}
    ${GL_LIBRARY_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set(INCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/build
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${GUACAMOLE_INCLUDE_DIRS}
    ${GUACAMOLE_GUI_INCLUDE_DIRS}
    ${GUACAMOLE_VOLUME_INCLUDE_DIRS}
    ${GUACAMOLE_PLOD_INCLUDE_DIRS}
    ${GL_INCLUDE_DIRS}
)

if (AVANGO_DISTRIBUTION_SUPPORT)
    list(APPEND LIB_PATHS ${ZMQ_LIBRARY_DIRS})
    list(APPEND INCLUDE_PATHS ${ZMQ_INCLUDE_DIRS})
endif (AVANGO_DISTRIBUTION_SUPPORT)

if (AVANGO_OCULUS_RIFT_SUPPORT)
  list(APPEND LIB_PATHS ${OVR_LIBRARY_DIRS})
  list(APPEND INCLUDE_PATHS ${OVR_INCLUDE_DIRS})
endif(AVANGO_OCULUS_RIFT_SUPPORT)

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
    set(CMAKE_CXX_FLAGS_RELEASE "-s -O4 --std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++11")
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
option (AVANGO_DISTRIBUTION_SUPPORT "Set to enable distribution support." ON)
option (AVANGO_PBR_SUPPORT "Set to enable point based rendering support.")
option (AVANGO_PCL_SUPPORT "Use PCL.")
option (AVANGO_UNITTESTS "Compile Unittests for Avango.")
option (AVANGO_OCULUS_RIFT_SUPPORT "Compile Oculus Rift module for Avango.")
option (AVANGO_VOLUME_SUPPORT "Compile Volume module for Avango.")
option (AVANGO_SKELANIM_SUPPORT "Compile Skeletal animation module for Avango.")
option (AVANGO_OPENAL_SUPPORT "Enable building OpenAL sound module.")
option (AVANGO_AWESOMIUM_SUPPORT "Enable building Awesomium module.")
option (AVANGO_DISPLAY_SUPPORT "Enable building display setup module.")
option (AVANGO_PHYSICS_SUPPORT "Set to enable physics support.")
option (AVANGO_RUNTIME_PROGRAM_COMPILATION "Set to enable runtime program compilation." ON)
option (AVANGO_VIDEO3D_SUPPORT "Compile Video3d module.")

IF (AVANGO_RUNTIME_PROGRAM_COMPILATION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGUACAMOLE_RUNTIME_PROGRAM_COMPILATION")
ENDIF (AVANGO_RUNTIME_PROGRAM_COMPILATION)

################################################################
# Create libraries
################################################################
file(MAKE_DIRECTORY ${AVANGO_INSTALL_DIRECTORY})
file(MAKE_DIRECTORY ${AVANGO_INSTALL_DIRECTORY}/python3.4)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(avango-unittest)
add_subdirectory(avango-core)
add_subdirectory(avango-python)
add_subdirectory(avango-gua)
add_subdirectory(avango-daemon)
add_subdirectory(avango-tools)
add_subdirectory(avango-sound)
add_subdirectory(avango-utils)

if (AVANGO_OCULUS_RIFT_SUPPORT)
    add_subdirectory(avango-oculus)
endif(AVANGO_OCULUS_RIFT_SUPPORT)

if (AVANGO_SKELANIM_SUPPORT)
    add_subdirectory(avango-skelanim)
endif(AVANGO_SKELANIM_SUPPORT)

if (AVANGO_OPENAL_SUPPORT)
    add_subdirectory(avango-openal)
endif(AVANGO_OPENAL_SUPPORT)

if (AVANGO_AWESOMIUM_SUPPORT)
    add_subdirectory(avango-awesomium)
endif(AVANGO_AWESOMIUM_SUPPORT)

if (AVANGO_VOLUME_SUPPORT)
    add_subdirectory(avango-volume)
endif(AVANGO_VOLUME_SUPPORT)

################################################################
# Summary
################################################################

message(STATUS "" )
message(STATUS "Summary:" )
message(STATUS " build type: ${CMAKE_BUILD_TYPE}" )
message(STATUS "" )
message(STATUS " boost:" )
message(STATUS "   library: ${Boost_LIBRARIES}" )
message(STATUS "   library path: ${Boost_LIBRARY_DIRS}" )
message(STATUS "   include: ${Boost_INCLUDE_DIRS}" )
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

if ( AVANGO_VOLUME_SUPPORT  )
  message(STATUS " guacamole-volume:" )
  message(STATUS "   library: ${GUACAMOLE_VOLUME_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_VOLUME_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_VOLUME_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_VOLUME_SUPPORT )

if ( AVANGO_AWESOMIUM_SUPPORT  )
  message(STATUS " guacamole-gui:" )
  message(STATUS "   library: ${GUACAMOLE_GUI_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_GUI_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_GUI_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_AWESOMIUM_SUPPORT )

if ( AVANGO_SKELANIM_SUPPORT  )
  message(STATUS " guacamole-oculus:" )
  message(STATUS "   library: ${GUACAMOLE_SKELANIM_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_SKELANIM_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_SKELANIM_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_SKELANIM_SUPPORT )

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
