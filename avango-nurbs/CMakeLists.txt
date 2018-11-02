###############################################################################
# determine source and header files
###############################################################################
file(GLOB_RECURSE AVANGO_NURBS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/gua/nurbs/scenegraph/NURBSNode.cpp
  src/avango/gua/nurbs/renderer/NURBSLoader.cpp
  src/avango/gua/nurbs/renderer/NURBSPassDescription.cpp
  src/avango/gua/nurbs/Init.cpp
  include/avango/gua/nurbs/scenegraph/NURBSNode.hpp
  include/avango/gua/nurbs/renderer/NURBSLoader.hpp
  include/avango/gua/nurbs/renderer/NURBSPassDescription.hpp
  include/avango/gua/nurbs/Init.hpp
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
    ${GUACAMOLE_NURBS_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ../avango-gua/include
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY(avango_nurbs SHARED
  ${AVANGO_NURBS_SRC}
)

ADD_DEPENDENCIES ( avango_nurbs avango)
set_target_properties(avango_nurbs PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_NURBS_LIBRARY")
  set_target_properties(avango_nurbs PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

SET(_AVANGO_NURBS_LIBRARIES ${LIBRARIES} guacamole-nurbs)
LIST(APPEND _AVANGO_NURBS_LIBRARIES )

TARGET_LINK_LIBRARIES( avango_nurbs avango_gua avango_script avango ${_AVANGO_NURBS_LIBRARIES})

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_nurbs
    ${AVANGO_INSTALL_DIRECTORY}/avango/avango_nurbs.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_nurbs
    ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_nurbs.so)
ENDIF (MSVC)
