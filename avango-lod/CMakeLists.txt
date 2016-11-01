###############################################################################
# determine source and header files
###############################################################################
file(GLOB_RECURSE AVANGO_LOD_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/gua/lod/scenegraph/MLodNode.cpp
  include/avango/gua/lod/scenegraph/MLodNode.hpp
)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${PYTHON3_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${PYTHON3_INCLUDE_DIRS}
    ${GUACAMOLE_LOD_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ../avango-gua/include
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY(avango_lod SHARED
  ${AVANGO_LOD_SRC}
)

ADD_DEPENDENCIES ( avango_lod avango)
set_target_properties(avango_lod PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_LOD_LIBRARY")
  set_target_properties(avango_lod PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

SET(_AVANGO_LOD_LIBRARIES ${LIBRARIES} guacamole-lod)
LIST(APPEND _AVANGO_LOD_LIBRARIES )

TARGET_LINK_LIBRARIES( avango_lod
  debug avango_gua avango_script avango ${_AVANGO_LOD_LIBRARIES}
  optimized avango_gua avango_script avango ${_AVANGO_LOD_LIBRARIES})

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_lod
    ${AVANGO_INSTALL_DIRECTORY}/avango/avango_lod.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_lod
    ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_lod.so)
ENDIF (MSVC)
