###############################################################################
# determine source and header files
###############################################################################
file(GLOB_RECURSE AVANGO_NRP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        src/avango/gua/nrp/NRPNode.cpp
        include/avango/gua/nrp/NRPNode.hpp)

find_package(gazebo REQUIRED)
list(APPEND GAZEBO_INCLUDE_DIRS "/usr/include/gazebo-7")

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
        ${LIB_PATHS}
        ${PYTHON3_LIBRARY_DIRS}
        ${BULLET_LIBRARY_DIRS}
        ${GAZEBO_LIBRARIES})

INCLUDE_DIRECTORIES(
        ${INCLUDE_PATHS}
        ${SCHISM_INCLUDE_DIRS}
        ${BULLET_INCLUDE_DIRS}
        ${PYTHON3_INCLUDE_DIRS}
        ${GUACAMOLE_NRP_INCLUDE_DIRS}
        ${GAZEBO_INCLUDE_DIRS}
        include
        ../avango-core/include
        ../avango-python/include
        ../avango-gua/include)

IF (AVANGO_DISTRIBUTION_SUPPORT)
    include_directories(${ZMQ_INCLUDE_DIRS})
ENDIF ()

ADD_LIBRARY(avango_nrp SHARED ${AVANGO_NRP_SRC})

ADD_DEPENDENCIES(avango_nrp avango)
set_target_properties(avango_nrp PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
    SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_NRP_LIBRARY")
    set_target_properties(avango_nrp PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

SET(_AVANGO_NRP_LIBRARIES ${LIBRARIES} guacamole-nrp)
LIST(APPEND _AVANGO_NRP_LIBRARIES)

TARGET_LINK_LIBRARIES(avango_nrp
        debug avango_gua avango_script avango ${_AVANGO_NRP_LIBRARIES}
        optimized avango_gua avango_script avango ${_AVANGO_NRP_LIBRARIES})

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
    post_build_install_target(avango_nrp ${AVANGO_INSTALL_DIRECTORY}/avango/avango_nrp.dll)
ELSEIF (UNIX)
    post_build_install_target(avango_nrp ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_nrp.so)
ENDIF (MSVC)
