################################################################
# Project GUACAMOLE
################################################################

PROJECT(AVANGO-GUA CXX) 

# We require at least version 2.8.0
cmake_minimum_required(VERSION 2.8.0)

find_package(PkgConfig)

# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

################################################################
# Configure and find libraries
################################################################

pkg_check_modules(ASSIMP REQUIRED assimp)
pkg_check_modules(JSON REQUIRED jsoncpp)
pkg_check_modules(GL REQUIRED gl)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(PYTHON REQUIRED python)

include(find_schism)
include(find_boost)
include(find_ev)
include(find_zmq)
include(find_protobuf)
include(find_bullet)
include(find_guacamole)
include(find_avango)

set(LIBS
    ${SCHISM_LIBRARIES}
    ${BOOST_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${JSON_LIBRARIES}
    ${GL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${EV_LIBRARIES}
    ${ZMQ_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${GUA_LIBRARIES}
    ${AV_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
link_libraries(${LIBS})

set(LIB_PATHS
    ${ASSIMP_LIBRARY_DIRS}
    ${JSON_LIBRARY_DIRS}
    ${GL_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
    ${PYTHON_LIBRARY_DIRS}
)
link_directories(${LIB_PATHS})

set(INCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SCHISM_INCLUDE_DIRS}
    ${BOOST_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${GL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${EV_INCLUDE_DIRS}
    ${ZMQ_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${GUA_INCLUDE_DIRS}
    ${AV_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)
include_directories(${INCLUDE_PATHS})

################################################################
# Create libraries
################################################################

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-s -O4 --std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++0x")

# avango-gua
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_subdirectory(src)

# avango-gua-python
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/avango/gua)
add_subdirectory(python)

################################################################
# Create examples
################################################################

file(GLOB _EXAMPLES ${CMAKE_CURRENT_SOURCE_DIR}/examples/*)
FOREACH(_EXAMPLE ${_EXAMPLES})
    if(EXISTS "${_EXAMPLE}/CMakeLists.txt")
      add_subdirectory(${_EXAMPLE})
    endif()
ENDFOREACH(_EXAMPLE ${_EXAMPLES})

################################################################
# Summary
################################################################

message( "" )
message( "Summary:" )
message( " build type: ${CMAKE_BUILD_TYPE}" )
message( "" )
message( " schism:" )
message( "   library: ${SCHISM_LIBRARIES}" )
message( "   include: ${SCHISM_INCLUDE_DIRS}" )
message( "" )
message( " boost:" )
message( "   library: ${BOOST_LIBRARIES}" )
message( "   include: ${BOOST_INCLUDE_DIRS}" )
message( "" )
message( " assimp:" )
message( "   library: ${ASSIMP_LIBRARIES}" )
message( "   include: ${ASSIMP_INCLUDE_DIRS}" )
message( "" )
message( " gl:" )
message( "   library: ${GL_LIBRARIES}" )
message( "   include: ${GL_INCLUDE_DIRS}" )
message( "" )
message( " glew:" )
message( "   library: ${GLEW_LIBRARIES}" )
message( "   include: ${GLEW_INCLUDE_DIRS}" )
message( "" )
message( " ev:" )
message( "   library: ${EV_LIBRARIES}" )
message( "   include: ${EV_INCLUDE_DIRS}" )
message( "" )
message( " zmq:" )
message( "   library: ${ZMQ_LIBRARIES}" )
message( "   include: ${ZMQ_INCLUDE_DIRS}" )
message( "" )
message( " protobuf:" )
message( "   library: ${PROTOBUF_LIBRARIES}" )
message( "   include: ${PROTOBUF_INCLUDE_DIRS}" )
message( "" )
message( " bullet:" )
message( "   library: ${BULLET_LIBRARIES}" )
message( "   include: ${BULLET_INCLUDE_DIRS}" )
message( "" )
message( " guacamole:" )
message( "   library: ${GUA_LIBRARIES}" )
message( "   include: ${GUA_INCLUDE_DIRS}" )
message( "" )
message( " avango:" )
message( "   library: ${AV_LIBRARIES}" )
message( "   include: ${AV_INCLUDE_DIRS}" )
message( "" )
message( " python:" )
message( "   library: ${PYTHON_LIBRARIES}" )
message( "   include: ${PYTHON_INCLUDE_DIRS}" )
message( "" )
